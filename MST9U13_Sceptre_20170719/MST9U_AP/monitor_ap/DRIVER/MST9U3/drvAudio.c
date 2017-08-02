

#include "Global.h"

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "drvAudio.h"
#include "regAUDIO.h"
#include "datatype.h"
#include "Global.h"
//-------------------------------------------------------------------------------------------------
#define MSAUDIO_DEBUG    0
#if ENABLE_MSTV_UART_DEBUG && MSAUDIO_DEBUG
#define MSAUDIO_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define MSAUDIO_PRINT(format, ...)
#endif

#if AudioFunc

//-------------------------------------------------------------------------------------------------
//  Control Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Customerize Defines
//-------------------------------------------------------------------------------------------------
#define AUDIO_STATE_DEBUG                       1
#define AUDIO_MUTE_DEBUG                        0


//-------------------------------------------------------------------------------------------------
//  Constant Defines
//-------------------------------------------------------------------------------------------------
#if EnableDACDePop
#define DAC_DELAY_STEP12                1
#define DAC_DELAY_STEP23                1
#define DAC_DELAY_STEP34                200//2000
#define DAC_DELAY_STEP45                50
#define DAC_DELAY_STEP56                100//300
#define DAC_DELAY_STEP67                200//900

#define WKUP_DELAY_STEP12                1
#define WKUP_DELAY_STEP23                1
#define WKUP_DELAY_STEP34                1
#define WKUP_DELAY_STEP45                1
#define WKUP_DELAY_STEP56                10
#define WKUP_DELAY_STEP67                100//300
#define WKUP_DELAY_STEP78                200//800

#define STDBY_DELAY_STEP12                1
#define STDBY_DELAY_STEP23                1
#define STDBY_DELAY_STEP34                1
#define STDBY_DELAY_STEP45                1
#define STDBY_DELAY_STEP56                1
#endif
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define AUDIO_VOLUME_ARRAY_NUMBER           115

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
code RegTbl2Type DEPOP_INIT_TBL[DE_POP_Finish][27]=
{
    {   //DE_POP_STEP1
        {REG_000B00  ,BIT0  ,BIT0 }, 	//PD_OPLP_EAR    0003
        {REG_000B00  ,BIT1  ,BIT1 }, 	//PD_VREF
        {REG_000B00  ,0x00  ,BIT2 }, 	//SEL_DEPOP_SB_EAR
        
        {REG_112CDA  ,BIT1  ,BIT1 },  	//PD_L0_DAC  ffff 
        {REG_112CDA  ,BIT5  ,BIT5 },  	//PD_R0_DAC
        {REG_112CDB  ,BIT1  ,BIT1 },  	//PD_REF_DAC
        {REG_112CDB  ,BIT2  ,BIT2 },  	//PD_BIAS_DAC
        {REG_112CDB  ,BIT4  ,BIT4 },  	//PD_IBIAS_EAR
        {REG_112CDB  ,BIT5  ,BIT5 },  	//PD_RT_EAR
        {REG_112CDB  ,BIT6  ,BIT6 },  	//PD_LT_EAR
        {REG_112CDC  ,BIT3  ,BIT3 },  	//PD_VI      000f
        
        {REG_112CDE  ,0x00  ,BIT2|BIT1 },	//SEL_TC[1:0] 0000
        {REG_112CDE  ,0x00  ,BIT6|BIT5 },	//SEL_DEPOP_EAR
        {REG_112CDE  ,0x00  ,BIT7 },		//SEL_IN_EAR
        {REG_112CDF  ,0x00  ,BIT0 },		//SEL_IN_EAR
        {REG_112CDF  ,0x00  ,BIT2|BIT1 },	//SEL_POPRES_EAR
        
        {REG_112CE4  ,0x00  ,BIT6 },     //EN_SW_DAC   0000 
        {REG_112CE4  ,0x00  ,BIT0 },     //EN_CK_DAC
        {REG_112CE5  ,0x00  ,BIT7 },     //EN_MSP
        {REG_112CE6  ,BIT3  ,BIT3 },     //EN_VMID2GND 0008
        
        {REG_112CE8  ,0x00  ,BIT0 },     //EN_STG2_AB_EAR  0000
        {REG_112CE8  ,0x00  ,BIT1 },     //EN_STG2_LP_EAR
        {REG_112CE8  ,0x00  ,BIT2 },     //EN_LTX_EAR
        {REG_112CE8  ,0x00  ,BIT3 },     //EN_RTX_EAR
        {REG_112CE8  ,0x00  ,BIT4 },     //EN_MUTE_EAR
        {REG_112CE9  ,0x00  ,BIT1 },     //EN_VREF_DISCH
        {REG_112CE9  ,0x00  ,0x7C },     //EN_VREF_SFTDCH
    },
    {   //DE_POP_STEP2
      //Address,Value,Mask
        {REG_000B00  ,BIT0  ,BIT0 }, 	//PD_OPLP_EAR    0003
        {REG_000B00  ,BIT1  ,BIT1 }, 	//PD_VREF
        {REG_000B00  ,0x00  ,BIT2 },	//SEL_DEPOP_SB_EAR
        
        {REG_112CDA  ,0x00  ,BIT1 },  	//PD_L0_DAC  ffff 
        {REG_112CDA  ,0x00  ,BIT5 },  	//PD_R0_DAC
        {REG_112CDB  ,0x00  ,BIT1 },  	//PD_REF_DAC
        {REG_112CDB  ,0x00  ,BIT2 },  	//PD_BIAS_DAC
        {REG_112CDB  ,BIT4  ,BIT4 },  	//PD_IBIAS_EAR
        {REG_112CDB  ,0x00  ,BIT5 },  	//PD_RT_EAR
        {REG_112CDB  ,0x00  ,BIT6 },  	//PD_LT_EAR
        {REG_112CDC  ,0x00  ,BIT3 },  	//PD_VI      000f
        
        {REG_112CDE  ,0x06  ,BIT2|BIT1 },	//SEL_TC[1:0] 0000
        {REG_112CDE  ,0x20  ,BIT6|BIT5 },	//SEL_DEPOP_EAR
        {REG_112CDE  ,0x00  ,BIT7 },		//SEL_IN_EAR
        {REG_112CDF  ,0x00  ,BIT0 },		//SEL_IN_EAR
        {REG_112CDF  ,BIT2  ,BIT2|BIT1 },//SEL_POPRES_EAR
        
        {REG_112CE4  ,BIT6  ,BIT6 },     //EN_SW_DAC   0000 
        {REG_112CE4  ,BIT0  ,BIT0 },     //EN_CK_DAC
        {REG_112CE5  ,0x80  ,BIT7 },     //EN_MSP
        {REG_112CE6  ,0x00  ,BIT3 },     //EN_VMID2GND 0008
        
        {REG_112CE8  ,0x00  ,BIT0 },     //EN_STG2_AB_EAR  0000
        {REG_112CE8  ,0x00  ,BIT1 },     //EN_STG2_LP_EAR
        {REG_112CE8  ,0x00  ,BIT2 },     //EN_LTX_EAR
        {REG_112CE8  ,0x00  ,BIT3 },     //EN_RTX_EAR
        {REG_112CE8  ,0x00  ,BIT4 },     //EN_MUTE_EAR
        {REG_112CE9  ,0x00  ,BIT1 },     //EN_VREF_DISCH
        {REG_112CE9  ,0x00  ,0x7C },     //EN_VREF_SFTDCH
    },
    {   //DE_POP_STEP3
      //Address,Value,Mask
        {REG_000B00  ,0x00  ,BIT0 }, 	//PD_OPLP_EAR    0003
        {REG_000B00  ,0x00  ,BIT1 }, 	//PD_VREF
        {REG_000B00  ,0x00  ,BIT2 },	//SEL_DEPOP_SB_EAR
        
        {REG_112CDA  ,0x00  ,BIT1 },  	//PD_L0_DAC  ffff 
        {REG_112CDA  ,0x00  ,BIT5 },  	//PD_R0_DAC
        {REG_112CDB  ,0x00  ,BIT1 },  	//PD_REF_DAC
        {REG_112CDB  ,0x00  ,BIT2 },  	//PD_BIAS_DAC
        {REG_112CDB  ,BIT4  ,BIT4 },  	//PD_IBIAS_EAR
        {REG_112CDB  ,0x00  ,BIT5 },  	//PD_RT_EAR
        {REG_112CDB  ,0x00  ,BIT6 },  	//PD_LT_EAR
        {REG_112CDC  ,0x00  ,BIT3 },  	//PD_VI      000f
        
        {REG_112CDE  ,0x06  ,BIT2|BIT1 },	//SEL_TC[1:0] 0000
        {REG_112CDE  ,0x20  ,BIT6|BIT5 },	//SEL_DEPOP_EAR
        {REG_112CDE  ,0x00  ,BIT7 },	//SEL_IN_EAR
        {REG_112CDF  ,0x00  ,BIT0 },	//SEL_IN_EAR
        {REG_112CDF  ,BIT2  ,BIT2|BIT1 },//SEL_POPRES_EAR
        
        {REG_112CE4  ,BIT6  ,BIT6 },     //EN_SW_DAC   0000 
        {REG_112CE4  ,BIT0  ,BIT0 },     //EN_CK_DAC
        {REG_112CE5  ,0x80  ,BIT7 },     //EN_MSP
        {REG_112CE6  ,0x00  ,BIT3 },     //EN_VMID2GND 0008
        
        {REG_112CE8  ,0x00  ,BIT0 },     //EN_STG2_AB_EAR  0000
        {REG_112CE8  ,0x00  ,BIT1 },     //EN_STG2_LP_EAR
        {REG_112CE8  ,0x00  ,BIT2 },     //EN_LTX_EAR
        {REG_112CE8  ,0x00  ,BIT3 },     //EN_RTX_EAR
        {REG_112CE8  ,0x00  ,BIT4 },     //EN_MUTE_EAR
        {REG_112CE9  ,0x00  ,BIT1 },     //EN_VREF_DISCH
        {REG_112CE9  ,0x00  ,0x7C },     //EN_VREF_SFTDCH    
    },
    {   //DE_POP_STEP4
      //Address,Value,Mask
        {REG_000B00  ,0x00  ,BIT0 }, //PD_OPLP_EAR    0003
        {REG_000B00  ,0x00  ,BIT1 }, //PD_VREF
        {REG_000B00  ,0x00  ,BIT2 },//SEL_DEPOP_SB_EAR
        
        {REG_112CDA  ,0x00  ,BIT1 },  //PD_L0_DAC  ffff 
        {REG_112CDA  ,0x00  ,BIT5 },  //PD_R0_DAC
        {REG_112CDB  ,0x00  ,BIT1 },  //PD_REF_DAC
        {REG_112CDB  ,0x00  ,BIT2 },  //PD_BIAS_DAC
        {REG_112CDB  ,0x00  ,BIT4 },  //PD_IBIAS_EAR
        {REG_112CDB  ,0x00  ,BIT5 },  //PD_RT_EAR
        {REG_112CDB  ,0x00  ,BIT6 },  //PD_LT_EAR
        {REG_112CDC  ,0x00  ,BIT3 },  //PD_VI      000f
        
        {REG_112CDE  ,0x06  ,BIT2|BIT1 },//SEL_TC[1:0] 0000
        {REG_112CDE  ,0x20  ,BIT6|BIT5 },//SEL_DEPOP_EAR
        {REG_112CDE  ,0x80  ,BIT7 },//SEL_IN_EAR
        {REG_112CDF  ,0x00  ,BIT0 },//SEL_IN_EAR
        {REG_112CDF  ,BIT2  ,BIT2|BIT1 },//SEL_POPRES_EAR
        
        {REG_112CE4  ,BIT6  ,BIT6 },     //EN_SW_DAC   0000 
        {REG_112CE4  ,BIT0  ,BIT0 },     //EN_CK_DAC
        {REG_112CE5  ,0x80  ,BIT7 },     //EN_MSP
        {REG_112CE6  ,0x00  ,BIT3 },     //EN_VMID2GND 0008
        
        {REG_112CE8  ,0x00  ,BIT0 },     //EN_STG2_AB_EAR  0000
        {REG_112CE8  ,BIT1  ,BIT1 },     //EN_STG2_LP_EAR
        {REG_112CE8  ,0x00  ,BIT2 },     //EN_LTX_EAR
        {REG_112CE8  ,0x00  ,BIT3 },     //EN_RTX_EAR
        {REG_112CE8  ,0x00  ,BIT4 },     //EN_MUTE_EAR
        {REG_112CE9  ,0x00  ,BIT1 },     //EN_VREF_DISCH
        {REG_112CE9  ,0x00  ,0x7C },     //EN_VREF_SFTDCH    
    },
    {   //DE_POP_STEP5
      //Address,Value,Mask
        {REG_000B00  ,0x00  ,BIT0 }, //PD_OPLP_EAR    0003
        {REG_000B00  ,0x00  ,BIT1 }, //PD_VREF
        {REG_000B00  ,0x00  ,BIT2 },//SEL_DEPOP_SB_EAR
        
        {REG_112CDA  ,0x00  ,BIT1 },  //PD_L0_DAC  ffff 
        {REG_112CDA  ,0x00  ,BIT5 },  //PD_R0_DAC
        {REG_112CDB  ,0x00  ,BIT1 },  //PD_REF_DAC
        {REG_112CDB  ,0x00  ,BIT2 },  //PD_BIAS_DAC
        {REG_112CDB  ,0x00  ,BIT4 },  //PD_IBIAS_EAR
        {REG_112CDB  ,0x00  ,BIT5 },  //PD_RT_EAR
        {REG_112CDB  ,0x00  ,BIT6 },  //PD_LT_EAR
        {REG_112CDC  ,0x00  ,BIT3 },  //PD_VI      000f
        
        {REG_112CDE  ,0x06  ,BIT2|BIT1 },//SEL_TC[1:0] 0000
        {REG_112CDE  ,0x20  ,BIT6|BIT5 },//SEL_DEPOP_EAR
        {REG_112CDE  ,0x80  ,BIT7 },//SEL_IN_EAR
        {REG_112CDF  ,0x00  ,BIT0 },//SEL_IN_EAR
        {REG_112CDF  ,BIT2  ,BIT2|BIT1 },//SEL_POPRES_EAR
        
        {REG_112CE4  ,BIT6  ,BIT6 },     //EN_SW_DAC   0000 
        {REG_112CE4  ,BIT0  ,BIT0 },     //EN_CK_DAC
        {REG_112CE5  ,0x80  ,BIT7 },     //EN_MSP
        {REG_112CE6  ,0x00  ,BIT3 },     //EN_VMID2GND 0008
        
        {REG_112CE8  ,0x00  ,BIT0 },     //EN_STG2_AB_EAR  0000
        {REG_112CE8  ,BIT1  ,BIT1 },     //EN_STG2_LP_EAR
        {REG_112CE8  ,BIT2  ,BIT2 },     //EN_LTX_EAR
        {REG_112CE8  ,BIT3  ,BIT3 },     //EN_RTX_EAR
        {REG_112CE8  ,0x00  ,BIT4 },     //EN_MUTE_EAR
        {REG_112CE9  ,0x00  ,BIT1 },     //EN_VREF_DISCH
        {REG_112CE9  ,0x00  ,0x7C },     //EN_VREF_SFTDCH    
    },
    {   //DE_POP_STEP6
      //Address,Value,Mask
        {REG_000B00  ,0x00  ,BIT0 }, //PD_OPLP_EAR    0003
        {REG_000B00  ,0x00  ,BIT1 }, //PD_VREF
        {REG_000B00  ,0x00  ,BIT2 },//SEL_DEPOP_SB_EAR
        
        {REG_112CDA  ,0x00  ,BIT1 },  //PD_L0_DAC  ffff 
        {REG_112CDA  ,0x00  ,BIT5 },  //PD_R0_DAC
        {REG_112CDB  ,0x00  ,BIT1 },  //PD_REF_DAC
        {REG_112CDB  ,0x00  ,BIT2 },  //PD_BIAS_DAC
        {REG_112CDB  ,0x00  ,BIT4 },  //PD_IBIAS_EAR
        {REG_112CDB  ,0x00  ,BIT5 },  //PD_RT_EAR
        {REG_112CDB  ,0x00  ,BIT6 },  //PD_LT_EAR
        {REG_112CDC  ,0x00  ,BIT3 },  //PD_VI      000f
        
        {REG_112CDE  ,0x06  ,BIT2|BIT1 },//SEL_TC[1:0] 0000
        {REG_112CDE  ,0x60  ,BIT6|BIT5 },//SEL_DEPOP_EAR
        {REG_112CDE  ,0x80  ,BIT7 },//SEL_IN_EAR
        {REG_112CDF  ,0x00  ,BIT0 },//SEL_IN_EAR
        {REG_112CDF  ,BIT2  ,BIT2|BIT1 },//SEL_POPRES_EAR
        
        {REG_112CE4  ,BIT6  ,BIT6 },     //EN_SW_DAC   0000 
        {REG_112CE4  ,BIT0  ,BIT0 },     //EN_CK_DAC
        {REG_112CE5  ,BIT7  ,BIT7 },     //EN_MSP
        {REG_112CE6  ,0x00  ,BIT3 },     //EN_VMID2GND 0008
        
        {REG_112CE8  ,0x00  ,BIT0 },     //EN_STG2_AB_EAR  0000
        {REG_112CE8  ,BIT1  ,BIT1 },     //EN_STG2_LP_EAR
        {REG_112CE8  ,BIT2  ,BIT2 },     //EN_LTX_EAR
        {REG_112CE8  ,BIT3  ,BIT3 },     //EN_RTX_EAR
        {REG_112CE8  ,0x00  ,BIT4 },     //EN_MUTE_EAR
        {REG_112CE9  ,0x00  ,BIT1 },     //EN_VREF_DISCH
        {REG_112CE9  ,0x00  ,0x7C },     //EN_VREF_SFTDCH   
    },
    {   //DE_POP_STEP7
      //Address,Value,Mask
        {REG_000B00  ,0x00  ,BIT0 }, //PD_OPLP_EAR    0003
        {REG_000B00  ,0x00  ,BIT1 }, //PD_VREF
        {REG_000B00  ,0x00  ,BIT2 },//SEL_DEPOP_SB_EAR
        
        {REG_112CDA  ,0x00  ,BIT1 },  //PD_L0_DAC  ffff 
        {REG_112CDA  ,0x00  ,BIT5 },  //PD_R0_DAC
        {REG_112CDB  ,0x00  ,BIT1 },  //PD_REF_DAC
        {REG_112CDB  ,0x00  ,BIT2 },  //PD_BIAS_DAC
        {REG_112CDB  ,0x00  ,BIT4 },  //PD_IBIAS_EAR
        {REG_112CDB  ,0x00  ,BIT5 },  //PD_RT_EAR
        {REG_112CDB  ,0x00  ,BIT6 },  //PD_LT_EAR
        {REG_112CDC  ,0x00  ,BIT3 },  //PD_VI      000f
        
        {REG_112CDE  ,0x06  ,BIT2|BIT1 },//SEL_TC[1:0] 0000
        {REG_112CDE  ,0x60  ,BIT6|BIT5 },//SEL_DEPOP_EAR
        {REG_112CDE  ,0x80  ,BIT7 },//SEL_IN_EAR
        {REG_112CDF  ,0x00  ,BIT0 },//SEL_IN_EAR
        {REG_112CDF  ,BIT2  ,BIT2|BIT1 },//SEL_POPRES_EAR
        
        {REG_112CE4  ,BIT6  ,BIT6 },     //EN_SW_DAC   0000 
        {REG_112CE4  ,BIT0  ,BIT0 },     //EN_CK_DAC
        {REG_112CE5  ,0x00  ,BIT7 },     //EN_MSP
        {REG_112CE6  ,0x00  ,BIT3 },     //EN_VMID2GND 0008
        
        {REG_112CE8  ,0x00  ,BIT0 },     //EN_STG2_AB_EAR  0000
        {REG_112CE8  ,BIT1  ,BIT1 },     //EN_STG2_LP_EAR
        {REG_112CE8  ,BIT2  ,BIT2 },     //EN_LTX_EAR
        {REG_112CE8  ,BIT3  ,BIT3 },     //EN_RTX_EAR
        {REG_112CE8  ,0x00  ,BIT4 },     //EN_MUTE_EAR
        {REG_112CE9  ,0x00  ,BIT1 },     //EN_VREF_DISCH
        {REG_112CE9  ,0x00  ,0x7C },     //EN_VREF_SFTDCH   
    },    
};

code RegTbl2Type WakeUpDepopSequence_TBL[WKUP_DE_POP_Finish][20]=
{
    {   //WAKEUP_DE_POP_STEP1
        //Address,Value,Mask
        {REG_112CDA,    (BIT1)  ,BIT1},     //PD_L0_DAC
        {REG_112CDA,    (BIT5)  ,BIT5},     //PD_R0_DAC
        {REG_112CDA,    (BIT9)  ,BIT9},     //PD_REF_DAC
        {REG_112CDA,    (BIT10) ,BIT10},        //PD_BIAS_DAC

        {REG_112CE4,    (0x00)  ,BIT6},     //EN_SW_DAC
        
        {REG_112CE4,    (0x00)  ,BIT0},     //EN_CK_DAC

        {REG_112CDC,    (BIT3)  ,BIT3},     //PD_VI
        {REG_000B00,    (0x00)  ,BIT1},     //PD_VREF

        {REG_000B00,    (0x00)  ,BIT0},        //PD_OPLP_EAR
        {REG_112CDA,    (BIT12) ,BIT12},        //PD_IBIAS_EAR
        {REG_112CDA,    (BIT13) ,BIT13},        //PD_RT_EAR
        {REG_112CDA,    (BIT14) ,BIT14},        //PD_LT_EAR

        {REG_112CE8,    (0x00)  ,BIT1},     //EN_STG2_LP_EAR
        {REG_112CE8,    (0x00)  ,BIT0},     //EN_STG2_AB_EAR

        {REG_000B00,    (BIT2) ,BIT2},        //SEL_DEPOP_SB_EAR
        {REG_112CDE,    (0x00)  ,BIT6|BIT5},        //SEL_DEPOP_EAR

        {REG_112CE8,    (0x00)  ,BIT3},     //EN_RTX_EAR
        {REG_112CE8,    (0x00)  ,BIT2},     //EN_LTX_EAR

        {REG_112CDE,    (0x00)  ,BIT10|BIT9},       //SEL_POPRES_EAR
        {REG_112CDE,    (0x00)  ,BIT8|BIT7},        //SEL_IN_EAR
    },
    {
        //WAKEUP_DE_POP_STEP2
        //Address,Value,Mask
        {REG_112CDA,    (BIT1)  ,BIT1},     //PD_L0_DAC
        {REG_112CDA,    (BIT5)  ,BIT5},     //PD_R0_DAC
        {REG_112CDA,    (BIT9)  ,BIT9},     //PD_REF_DAC
        {REG_112CDA,    (BIT10) ,BIT10},        //PD_BIAS_DAC

        {REG_112CE4,    (0x00)  ,BIT6},     //EN_SW_DAC

        {REG_112CE4,    (BIT0)  ,BIT0},     //EN_CK_DAC

        {REG_112CDC,    (BIT3)  ,BIT3},     //PD_VI
        {REG_000B00,    (0x00)  ,BIT1},     //PD_VREF

        {REG_000B00,    (0x00)  ,BIT0},        //PD_OPLP_EAR
        {REG_112CDA,    (BIT12) ,BIT12},        //PD_IBIAS_EAR
        {REG_112CDA,    (BIT13) ,BIT13},        //PD_RT_EAR
        {REG_112CDA,    (BIT14) ,BIT14},        //PD_LT_EAR

        {REG_112CE8,    (0x00)  ,BIT1},     //EN_STG2_LP_EAR
        {REG_112CE8,    (0x00)  ,BIT0},     //EN_STG2_AB_EAR

        {REG_000B00,    (BIT2) ,BIT2},        //SEL_DEPOP_SB_EAR
        {REG_112CDE,    (0x00) ,BIT6|BIT5},        //SEL_DEPOP_EAR

        {REG_112CE8,    (0x00)  ,BIT3},     //EN_RTX_EAR
        {REG_112CE8,    (0x00)  ,BIT2},     //EN_LTX_EAR

        {REG_112CDE,    (BIT10) ,BIT10|BIT9},       //SEL_POPRES_EAR
        {REG_112CDE,    (BIT7)  ,BIT8|BIT7},        //SEL_IN_EAR
    },
    {
        //WAKEUP_DE_POP_STEP3
        //Address,Value,Mask
        {REG_112CDA,    (0x00)  ,BIT1},     //PD_L0_DAC
        {REG_112CDA,    (0x00)  ,BIT5},     //PD_R0_DAC
        {REG_112CDA,    (0x00)  ,BIT9},     //PD_REF_DAC
        {REG_112CDA,    (0x00)  ,BIT10},        //PD_BIAS_DAC

        {REG_112CE4,    (0x00)  ,BIT6},     //EN_SW_DAC

        {REG_112CE4,    (BIT0)  ,BIT0},     //EN_CK_DAC

        {REG_112CDC,    (0x00)  ,BIT3},     //PD_VI
        {REG_000B00,    (0x00)  ,BIT1},     //PD_VREF

        {REG_000B00,    (0x00)  ,BIT0},        //PD_OPLP_EAR
        {REG_112CDA,    (BIT12) ,BIT12},        //PD_IBIAS_EAR
        {REG_112CDA,    (BIT13) ,BIT13},        //PD_RT_EAR
        {REG_112CDA,    (BIT14) ,BIT14},        //PD_LT_EAR

        {REG_112CE8,    (0x00)  ,BIT1},     //EN_STG2_LP_EAR
        {REG_112CE8,    (0x00)  ,BIT0},     //EN_STG2_AB_EAR

        {REG_000B00,    (BIT2) ,BIT2},        //SEL_DEPOP_SB_EAR
        {REG_112CDE,    (0x00) ,BIT6|BIT5},        //SEL_DEPOP_EAR

        {REG_112CE8,    (0x00)  ,BIT3},     //EN_RTX_EAR
        {REG_112CE8,    (0x00)  ,BIT2},     //EN_LTX_EAR

        {REG_112CDE,    (BIT10) ,BIT10|BIT9},       //SEL_POPRES_EAR
        {REG_112CDE,    (BIT7)  ,BIT8|BIT7},        //SEL_IN_EAR
    },
    {
        //WAKEUP_DE_POP_STEP4
        //Address,Value,Mask
        {REG_112CDA,    (0x00)  ,BIT1},     //PD_L0_DAC
        {REG_112CDA,    (0x00)  ,BIT5},     //PD_R0_DAC
        {REG_112CDA,    (0x00)  ,BIT9},     //PD_REF_DAC
        {REG_112CDA,    (0x00)  ,BIT10},        //PD_BIAS_DAC

        {REG_112CE4,    (BIT6)  ,BIT6},     //EN_SW_DAC
        {REG_112CE4,    (BIT0)  ,BIT0},     //EN_CK_DAC

        {REG_112CDC,    (0x00)  ,BIT3},     //PD_VI
        {REG_000B00,    (0x00)  ,BIT1},     //PD_VREF

        {REG_000B00,    (0x00)  ,BIT0},        //PD_OPLP_EAR
        {REG_112CDA,    (0x00)  ,BIT12},        //PD_IBIAS_EAR
        {REG_112CDA,    (0x00)  ,BIT13},        //PD_RT_EAR
        {REG_112CDA,    (0x00)  ,BIT14},        //PD_LT_EAR

        {REG_112CE8,    (0x00)  ,BIT1},     //EN_STG2_LP_EAR
        {REG_112CE8,    (0x00)  ,BIT0},     //EN_STG2_AB_EAR

        {REG_000B00,    (0x00)  ,BIT2},        //SEL_DEPOP_SB_EAR
        {REG_112CDE,    (0x00) ,BIT6|BIT5},        //SEL_DEPOP_EAR

        {REG_112CE8,    (0x00)  ,BIT3},     //EN_RTX_EAR
        {REG_112CE8,    (0x00)  ,BIT2},     //EN_LTX_EAR

        {REG_112CDE,    (BIT10) ,BIT10|BIT9},       //SEL_POPRES_EAR
        {REG_112CDE,    (BIT7)  ,BIT8|BIT7},        //SEL_IN_EAR
    },
    {
        //WAKEUP_DE_POP_STEP5
        //Address,Value,Mask
        {REG_112CDA,    (0x00)  ,BIT1},     //PD_L0_DAC
        {REG_112CDA,    (0x00)  ,BIT5},     //PD_R0_DAC
        {REG_112CDA,    (0x00)  ,BIT9},     //PD_REF_DAC
        {REG_112CDA,    (0x00)  ,BIT10},        //PD_BIAS_DAC

        {REG_112CE4,    (BIT6)  ,BIT6},     //EN_SW_DAC
        {REG_112CE4,    (BIT0)  ,BIT0},     //EN_CK_DAC

        {REG_112CDC,    (0x00)  ,BIT3},     //PD_VI
        {REG_000B00,    (0x00)  ,BIT1},     //PD_VREF

        {REG_000B00,    (0x00)  ,BIT0},        //PD_OPLP_EAR
        {REG_112CDA,    (0x00)  ,BIT12},        //PD_IBIAS_EAR
        {REG_112CDA,    (0x00)  ,BIT13},        //PD_RT_EAR
        {REG_112CDA,    (0x00)  ,BIT14},        //PD_LT_EAR

        {REG_112CE8,    (BIT1)  ,BIT1},     //EN_STG2_LP_EAR
        {REG_112CE8,    (0x00)  ,BIT0},     //EN_STG2_AB_EAR

        {REG_000B00,    (0x00)  ,BIT2},        //SEL_DEPOP_SB_EAR
        {REG_112CDE,    (0x00) ,BIT6|BIT5},        //SEL_DEPOP_EAR

        {REG_112CE8,    (0x00)  ,BIT3},     //EN_RTX_EAR
        {REG_112CE8,    (0x00)  ,BIT2},     //EN_LTX_EAR

        {REG_112CDE,    (BIT10) ,BIT10|BIT9},       //SEL_POPRES_EAR
        {REG_112CDE,    (BIT7)  ,BIT8|BIT7},        //SEL_IN_EAR
    },
    {
        //WAKEUP_DE_POP_STEP6
        //Address,Value,Mask
        {REG_112CDA,    (0x00)  ,BIT1},     //PD_L0_DAC
        {REG_112CDA,    (0x00)  ,BIT5},     //PD_R0_DAC
        {REG_112CDA,    (0x00)  ,BIT9},     //PD_REF_DAC
        {REG_112CDA,    (0x00)  ,BIT10},        //PD_BIAS_DAC

        {REG_112CE4,    (BIT6)  ,BIT6},     //EN_SW_DAC
        {REG_112CE4,    (BIT0)  ,BIT0},     //EN_CK_DAC

        {REG_112CDC,    (0x00)  ,BIT3},     //PD_VI
        {REG_000B00,    (0x00)  ,BIT1},     //PD_VREF

        {REG_000B00,    (0x00)  ,BIT0},        //PD_OPLP_EAR
        {REG_112CDA,    (0x00)  ,BIT12},        //PD_IBIAS_EAR
        {REG_112CDA,    (0x00)  ,BIT13},        //PD_RT_EAR
        {REG_112CDA,    (0x00)  ,BIT14},        //PD_LT_EAR

        {REG_112CE8,    (BIT1)  ,BIT1},     //EN_STG2_LP_EAR
        {REG_112CE8,    (0x00)  ,BIT0},     //EN_STG2_AB_EAR

        {REG_000B00,    (0x00)  ,BIT2},        //SEL_DEPOP_SB_EAR
        {REG_112CDE,    (0x00) ,BIT6|BIT5},        //SEL_DEPOP_EAR

        {REG_112CE8,    (BIT3)  ,BIT3},     //EN_RTX_EAR
        {REG_112CE8,    (BIT2)  ,BIT2},     //EN_LTX_EAR

        {REG_112CDE,    (BIT10) ,BIT10|BIT9},       //SEL_POPRES_EAR
        {REG_112CDE,    (BIT7)  ,BIT8|BIT7},        //SEL_IN_EAR
    },
    {
        //WAKEUP_DE_POP_STEP7
        //Address,Value,Mask
        {REG_112CDA,    (0x00)  ,BIT1},     //PD_L0_DAC
        {REG_112CDA,    (0x00)  ,BIT5},     //PD_R0_DAC
        {REG_112CDA,    (0x00)  ,BIT9},     //PD_REF_DAC
        {REG_112CDA,    (0x00)  ,BIT10},        //PD_BIAS_DAC

        {REG_112CE4,    (BIT6)  ,BIT6},     //EN_SW_DAC
        {REG_112CE4,    (BIT0)  ,BIT0},     //EN_CK_DAC

        {REG_112CDC,    (0x00)  ,BIT3},     //PD_VI
        {REG_000B00,    (0x00)  ,BIT1},     //PD_VREF

        {REG_000B00,    (0x00)  ,BIT0},        //PD_OPLP_EAR
        {REG_112CDA,    (0x00)  ,BIT12},        //PD_IBIAS_EAR
        {REG_112CDA,    (0x00)  ,BIT13},        //PD_RT_EAR
        {REG_112CDA,    (0x00)  ,BIT14},        //PD_LT_EAR

        {REG_112CE8,    (BIT1)  ,BIT1},     //EN_STG2_LP_EAR
        {REG_112CE8,    (0x00)  ,BIT0},     //EN_STG2_AB_EAR

        {REG_000B00,    (0x00)  ,BIT2},        //SEL_DEPOP_SB_EAR
        {REG_112CDE,    (BIT6|BIT5) ,BIT6|BIT5},        //SEL_DEPOP_EAR

        {REG_112CE8,    (BIT3)  ,BIT3},     //EN_RTX_EAR
        {REG_112CE8,    (BIT2)  ,BIT2},     //EN_LTX_EAR

        {REG_112CDE,    (BIT10) ,BIT10|BIT9},       //SEL_POPRES_EAR
        {REG_112CDE,    (BIT7)  ,BIT8|BIT7},        //SEL_IN_EAR
    },
    {
        //WAKEUP_DE_POP_STEP8
        //Address,Value,Mask
        {REG_112CDA,    (0x00)  ,BIT1},     //PD_L0_DAC
        {REG_112CDA,    (0x00)  ,BIT5},     //PD_R0_DAC
        {REG_112CDA,    (0x00)  ,BIT9},     //PD_REF_DAC
        {REG_112CDA,    (0x00)  ,BIT10},        //PD_BIAS_DAC

        {REG_112CE4,    (BIT6)  ,BIT6},     //EN_SW_DAC
        {REG_112CE4,    (BIT0)  ,BIT0},     //EN_CK_DAC

        {REG_112CDC,    (0x00)  ,BIT3},     //PD_VI
        {REG_000B00,    (0x00)  ,BIT1},     //PD_VREF

        {REG_000B00,    (0x00)  ,BIT0},        //PD_OPLP_EAR
        {REG_112CDA,    (0x00)  ,BIT12},        //PD_IBIAS_EAR
        {REG_112CDA,    (0x00)  ,BIT13},        //PD_RT_EAR
        {REG_112CDA,    (0x00)  ,BIT14},        //PD_LT_EAR

        {REG_112CE8,    (BIT1)  ,BIT1},     //EN_STG2_LP_EAR
        {REG_112CE8,    (0)  ,BIT0},     //EN_STG2_AB_EAR

        {REG_000B00,    (0x00)  ,BIT2},        //SEL_DEPOP_SB_EAR
        {REG_112CDE,    (BIT6|BIT5) ,BIT6|BIT5},        //SEL_DEPOP_EAR

        {REG_112CE8,    (BIT3)  ,BIT3},     //EN_RTX_EAR
        {REG_112CE8,    (BIT2)  ,BIT2},     //EN_LTX_EAR

        {REG_112CDE,    (BIT10) ,BIT10|BIT9},       //SEL_POPRES_EAR
        {REG_112CDE,    (BIT7)  ,BIT8|BIT7},        //SEL_IN_EAR
    },
    
};

code RegTbl2Type StandByDepopSequence_TBL[STDBY_DE_POP_Finish][20]=
{
    {   //STANDBY_DE_POP_STEP1
        //Address,Value,Mask
        {REG_112CDA,    (0x00)  ,BIT1},     //PD_L0_DAC
        {REG_112CDA,    (0x00)  ,BIT5},     //PD_R0_DAC
        {REG_112CDA,    (0x00)  ,BIT9},     //PD_REF_DAC
        {REG_112CDA,    (0x00) ,BIT10},    //PD_BIAS_DAC

        {REG_112CE4,    (BIT6)  ,BIT6},     //EN_SW_DAC
        {REG_112CE4,    (BIT0)  ,BIT0},     //EN_CK_DAC

        {REG_112CDC,    (0x00)  ,BIT3},     //PD_VI
        {REG_000B00,    (0x00)  ,BIT1},     //PD_VREF

        {REG_000B00,    (0x00) ,BIT0},    //PD_OPLP_EAR
        {REG_112CDA,    (0x00) ,BIT12},    //PD_IBIAS_EAR
        {REG_112CDA,    (0x00) ,BIT13},    //PD_RT_EAR
        {REG_112CDA,    (0x00) ,BIT14},    //PD_LT_EAR

        {REG_000B00,    (0x00)  ,BIT2},     //SEL_DEPOP_SB_EAR 
        {REG_112CE8,    (0x00)  ,BIT0},     //EN_STG2_AB_EAR

        {REG_112CE8,    (BIT1) ,BIT1},   //EN_STG2_LP_EAR
        {REG_112CDE,    (BIT6|BIT5)  ,BIT6|BIT5},        //SEL_DEPOP_EAR

        {REG_112CE8,    (BIT3)  ,BIT3},     //EN_RTX_EAR
        {REG_112CE8,    (BIT2)  ,BIT2},     //EN_LTX_EAR

        {REG_112CDE,    (BIT10)  ,BIT10|BIT9},       //SEL_POPRES_EAR
        {REG_112CDE,    (BIT7)  ,BIT8|BIT7},        //SEL_IN_EAR
    },
    {   //STANDBY_DE_POP_STEP2
        //Address,Value,Mask
        {REG_112CDA,    (0x00)  ,BIT1},     //PD_L0_DAC
        {REG_112CDA,    (0x00)  ,BIT5},     //PD_R0_DAC
        {REG_112CDA,    (0x00)  ,BIT9},     //PD_REF_DAC
        {REG_112CDA,    (0x00) ,BIT10},    //PD_BIAS_DAC

        {REG_112CE4,    (BIT6)  ,BIT6},     //EN_SW_DAC
        {REG_112CE4,    (BIT0)  ,BIT0},     //EN_CK_DAC

        {REG_112CDC,    (0x00)  ,BIT3},     //PD_VI
        {REG_000B00,    (0x00)  ,BIT1},     //PD_VREF

        {REG_000B00,    (0x00)  ,BIT0},        //PD_OPLP_EAR
        {REG_112CDA,    (0x00) ,BIT12},        //PD_IBIAS_EAR
        {REG_112CDA,    (0x00) ,BIT13},        //PD_RT_EAR
        {REG_112CDA,    (0x00) ,BIT14},        //PD_LT_EAR

        {REG_000B00,    (0x00)  ,BIT2},     //SEL_DEPOP_SB_EAR
        {REG_112CE8,    (0x00)  ,BIT0},     //EN_STG2_AB_EAR

        {REG_112CE8,    (BIT1) ,BIT1},         //EN_STG2_LP_EAR
        {REG_112CDE,    (BIT6|BIT5)  ,BIT6|BIT5},        //SEL_DEPOP_EAR

        {REG_112CE8,    (BIT3)  ,BIT3},     //EN_RTX_EAR
        {REG_112CE8,    (BIT2)  ,BIT2},     //EN_LTX_EAR

        {REG_112CDE,    (BIT10)  ,BIT10|BIT9},       //SEL_POPRES_EAR
        {REG_112CDE,    (BIT7)  ,BIT8|BIT7},        //SEL_IN_EAR
    },
    {   //STANDBY_DE_POP_STEP3
        //Address,Value,Mask
        {REG_112CDA,    (0x00)  ,BIT1},     //PD_L0_DAC
        {REG_112CDA,    (0x00)  ,BIT5},     //PD_R0_DAC
        {REG_112CDA,    (0x00)  ,BIT9},     //PD_REF_DAC
        {REG_112CDA,    (0x00) ,BIT10},        //PD_BIAS_DAC

        {REG_112CE4,    (BIT6)  ,BIT6},     //EN_SW_DAC
        {REG_112CE4,    (BIT0)  ,BIT0},     //EN_CK_DAC

        {REG_112CDC,    (0x00)  ,BIT3},     //PD_VI
        {REG_000B00,    (0x00)  ,BIT1},     //PD_VREF

        {REG_000B00,    (0x00)  ,BIT0},        //PD_OPLP_EAR
        {REG_112CDA,    (0x00) ,BIT12},        //PD_IBIAS_EAR
        {REG_112CDA,    (0x00) ,BIT13},        //PD_RT_EAR
        {REG_112CDA,    (0x00) ,BIT14},        //PD_LT_EAR

        {REG_000B00,    (0x00)  ,BIT2},    //SEL_DEPOP_SB_EAR 
        {REG_112CE8,    (0x00)  ,BIT0},     //EN_STG2_AB_EAR

        {REG_112CE8,    (BIT1) ,BIT1},         //EN_STG2_LP_EAR
        {REG_112CDE,    (BIT6|BIT5)  ,BIT6|BIT5},        //SEL_DEPOP_EAR

        {REG_112CE8,    (0x00)  ,BIT3},     //EN_RTX_EAR
        {REG_112CE8,    (0x00)  ,BIT2},     //EN_LTX_EAR

        {REG_112CDE,    (BIT10)  ,BIT10|BIT9},       //SEL_POPRES_EAR
        {REG_112CDE,    (BIT7)  ,BIT8|BIT7},        //SEL_IN_EAR
    },
    {   //STANDBY_DE_POP_STEP4
        //Address,Value,Mask
        {REG_112CDA,    (0x00)  ,BIT1},     //PD_L0_DAC
        {REG_112CDA,    (0x00)  ,BIT5},     //PD_R0_DAC
        {REG_112CDA,    (0x00)  ,BIT9},     //PD_REF_DAC
        {REG_112CDA,    (0x00) ,BIT10},        //PD_BIAS_DAC

        {REG_112CE4,    (BIT6)  ,BIT6},     //EN_SW_DAC
        {REG_112CE4,    (BIT0)  ,BIT0},     //EN_CK_DAC

        {REG_112CDC,    (0x00)  ,BIT3},     //PD_VI
        {REG_000B00,    (0x00)  ,BIT1},     //PD_VREF

        {REG_000B00,    (0x00)  ,BIT0},        //PD_OPLP_EAR
        {REG_112CDA,    (0x00) ,BIT12},        //PD_IBIAS_EAR
        {REG_112CDA,    (0x00) ,BIT13},        //PD_RT_EAR
        {REG_112CDA,    (0x00) ,BIT14},        //PD_LT_EAR

        {REG_000B00,    (BIT2)  ,BIT2},    //SEL_DEPOP_SB_EAR 
        {REG_112CE8,    (0x00)  ,BIT0},     //EN_STG2_AB_EAR

        {REG_112CE8,    (BIT1) ,BIT1},         //EN_STG2_LP_EAR
        {REG_112CDE,    (BIT6|BIT5)  ,BIT6|BIT5},        //SEL_DEPOP_EAR

        {REG_112CE8,    (0x00)  ,BIT3},     //EN_RTX_EAR
        {REG_112CE8,    (0x00)  ,BIT2},     //EN_LTX_EAR

        {REG_112CDE,    (BIT10)  ,BIT10|BIT9},       //SEL_POPRES_EAR
        {REG_112CDE,    (BIT7)  ,BIT8|BIT7},        //SEL_IN_EAR
    },
    {   //STANDBY_DE_POP_STEP5
        //Address,Value,Mask
        {REG_112CDA,    (0x00)  ,BIT1},     //PD_L0_DAC
        {REG_112CDA,    (0x00)  ,BIT5},     //PD_R0_DAC
        {REG_112CDA,    (0x00)  ,BIT9},     //PD_REF_DAC
        {REG_112CDA,    (0x00) ,BIT10},        //PD_BIAS_DAC

        {REG_112CE4,    (0x00)  ,BIT6},     //EN_SW_DAC 
        {REG_112CE4,    (BIT0)  ,BIT0},     //EN_CK_DAC 

        {REG_112CDC,    (0x00)  ,BIT3},     //PD_VI
        {REG_000B00,    (0x00)  ,BIT1},     //PD_VREF

        {REG_000B00,    (0x00)  ,BIT0},        //PD_OPLP_EAR
        {REG_112CDA,    (BIT12) ,BIT12},        //PD_IBIAS_EAR 
        {REG_112CDA,    (BIT13) ,BIT13},        //PD_RT_EAR 
        {REG_112CDA,    (BIT14) ,BIT14},        //PD_LT_EAR 

        {REG_000B00,    (BIT2)  ,BIT2},    //SEL_DEPOP_SB_EAR 
        {REG_112CE8,    (0x00)  ,BIT0},     //EN_STG2_AB_EAR

        {REG_112CE8,    (0x00) ,BIT1},         //EN_STG2_LP_EAR 
        {REG_112CDE,    (BIT6|BIT5)  ,BIT6|BIT5},        //SEL_DEPOP_EAR

        {REG_112CE8,    (0x00)  ,BIT3},     //EN_RTX_EAR
        {REG_112CE8,    (0x00)  ,BIT2},     //EN_LTX_EAR 

        {REG_112CDE,    (BIT10)  ,BIT10|BIT9},       //SEL_POPRES_EAR
        {REG_112CDE,    (BIT7)  ,BIT8|BIT7},        //SEL_IN_EAR
    },
    {   //STANDBY_DE_POP_STEP6
        //Address,Value,Mask
        {REG_112CDA,    (BIT1)  ,BIT1},     //PD_L0_DAC
        {REG_112CDA,    (BIT5)  ,BIT5},     //PD_R0_DAC
        {REG_112CDA,    (BIT9)  ,BIT9},     //PD_REF_DAC
        {REG_112CDA,    (BIT10) ,BIT10},        //PD_BIAS_DAC

        {REG_112CE4,    (0x00)  ,BIT6},     //EN_SW_DAC
        {REG_112CE4,    (0x00)  ,BIT0},     //EN_CK_DAC

        {REG_112CDC,    (BIT3)  ,BIT3},     //PD_VI
        {REG_000B00,    (0x00)  ,BIT1},     //PD_VREF

        {REG_000B00,    (0x00)  ,BIT0},        //PD_OPLP_EAR
        {REG_112CDA,    (BIT12) ,BIT12},        //PD_IBIAS_EAR
        {REG_112CDA,    (BIT13) ,BIT13},        //PD_RT_EAR
        {REG_112CDA,    (BIT14) ,BIT14},        //PD_LT_EAR

        {REG_000B00,    (BIT2)  ,BIT2},    //SEL_DEPOP_SB_EAR  
        {REG_112CE8,    (0x00)  ,BIT0},     //EN_STG2_AB_EAR

        {REG_112CE8,    (0x00) ,BIT1},         //EN_STG2_LP_EAR
        {REG_112CDE,    (0x00)  ,BIT6|BIT5},       //EN_STG2_LP_EAR

        {REG_112CE8,    (0x00)  ,BIT3},     //EN_RTX_EAR
        {REG_112CE8,    (0x00)  ,BIT2},     //EN_LTX_EAR

        {REG_112CDE,    (0x00)  ,BIT10|BIT9},       //SEL_POPRES_EAR
        {REG_112CDE,    (0x00)  ,BIT8|BIT7},        //SEL_IN_EAR
    },
};

#if !AudioNewCurve
const U16 AudVolumeTab[AUDIO_VOLUME_ARRAY_NUMBER] =
{
    //   0       1       2       3       4       5       6       7       8       9     OSD  Volume_Range
    0x0000, 0x0008, 0x0010, 0x0018, 0x0020, 0x0028, 0x0030, 0x0038, 0x0040, 0x0048, //  00,   0dB ~ -9dB
    0x0050, 0x0058, 0x0060, 0x0068, 0x0070, 0x0078, 0x0080, 0x0088, 0x0090, 0x0098, //  10, -10dB ~ -19dB
    0x00A0, 0x00A8, 0x00B0, 0x00B8, 0x00C0, 0x00C8, 0x00D0, 0x00D8, 0x00E0, 0x00E8, //  20, -20dB ~ -29dB
    0x00F0, 0x00F8, 0x0100, 0x0108, 0x0110, 0x0118, 0x0120, 0x0128, 0x0130, 0x0138, //  30, -30dB ~ -39dB
    0x0140, 0x0148, 0x0150, 0x0158, 0x0160, 0x0168, 0x0170, 0x0178, 0x0180, 0x0188, //  40, -40dB ~ -49dB
    0x0190, 0x0198, 0x01A0, 0x01A8, 0x01B0, 0x01B8, 0x01C0, 0x01C8, 0x01D0, 0x01D8, //  50, -50dB ~ -59dB
    0x01E0, 0x01E8, 0x01F0, 0x01F8, 0x0200, 0x0208, 0x0210, 0x0218, 0x0220, 0x0228, //  60, -60dB ~ -69dB
    0x0230, 0x0238, 0x0240, 0x0248, 0x0250, 0x0258, 0x0260, 0x0268, 0x0270, 0x0278, //  70, -70dB ~ -79dB
    0x0280, 0x0288, 0x0290, 0x0298, 0x02A0, 0x02A8, 0x02B0, 0x02B8, 0x02C0, 0x02C8, //  80, -80dB ~ -89dB
    0x02D0, 0x02D8, 0x02E0, 0x02E8, 0x02F0, 0x02F8, 0x0300, 0x0308, 0x0310, 0x0318, //  90, -90dB ~ -99dB
    0x0320, 0x0328, 0x0330, 0x0338, 0x0340, 0x0348, 0x0350, 0x0358, 0x0360, 0x0368, // 100,-100dB ~-109dB
    0x0370, 0x0378, 0x0380, 0x0388, 0x0390                                                         // 110,-110dB ~-114dB
};
#endif
//-------------------------------------------------------------------------------------------------
// Mute Defines
//-------------------------------------------------------------------------------------------------
#if ENABLE_DAC
static  BOOLEANS m_BOOLEANS_1;
#define m_bDPGA_InitAudioMute           m_BOOLEANS_1.bBOOL_0
#define m_bDPGA_SourceChangeAudioMute   m_BOOLEANS_1.bBOOL_1
#define m_bDPGA_MenuAudioMute           m_BOOLEANS_1.bBOOL_2
#define m_bDPGA_AVSyncAudioMute         m_BOOLEANS_1.bBOOL_3
#define m_bDPGA_TimingChangeAudioMute   m_BOOLEANS_1.bBOOL_4
#define m_bDPGA_ModeChangeAudioMute     m_BOOLEANS_1.bBOOL_5
#define m_bDPGA_DelayChangeAudioMute    m_BOOLEANS_1.bBOOL_6
#define m_bDPGA_Internal1AudioMute      m_BOOLEANS_1.bBOOL_7

static  BOOLEANS m_BOOLEANS_2;
#define m_bSW_MailboxAudioMute          m_BOOLEANS_2.bBOOL_0
#define m_bSW_SourceChangeAudioMute     m_BOOLEANS_2.bBOOL_1
#define m_bSW_InitAudioMute             m_BOOLEANS_2.bBOOL_2
#define m_bSW_MenuAudioMute             m_BOOLEANS_2.bBOOL_3
#define m_bSW_AVSyncAudioMute           m_BOOLEANS_2.bBOOL_4
#define m_bSW_TimingChangeAudioMute     m_BOOLEANS_2.bBOOL_5
#define m_bSW_ModeChangeAudioMute       m_BOOLEANS_2.bBOOL_6
#define m_bSW_Internal1AudioMute        m_BOOLEANS_2.bBOOL_7
#endif

//-------------------------------------------------------------------------------------------------
// Local Variable
//-------------------------------------------------------------------------------------------------
#if ENABLE_DAC
static MS_BOOL  audio_pad_init_flag         = 0;
static MS_BOOL  audio_dpga_mute_flag        = 0;
static U16      dpga0_volume_lt_store       = 0;
static U16      dpga0_volume_rt_store       = 0;
DAC_INIT_STEP   AudioDePopSequence = DE_POP_STEP1;
DAC_WKUP_STEP   AudioWKUPDePopSequence = WKUP_DE_POP_STEP1;

//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///           AUDIO DRV FUNCTION
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//******************************************************************************

#if EnableDACDePop
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudioPoweronDePop()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioPoweronDePop(DAC_INIT_STEP DePopSequence)
{
    BYTE uctemp=0;

    if (bAudioDePopFlag == 0)
        return;

    for(uctemp = 0; uctemp <(sizeof(DEPOP_INIT_TBL)/sizeof(RegTbl2Type)/DE_POP_Finish); uctemp++)
    {
            msWrite2ByteMask(DEPOP_INIT_TBL[DePopSequence][uctemp].dwReg, DEPOP_INIT_TBL[DePopSequence][uctemp].wValue ,DEPOP_INIT_TBL[DePopSequence][uctemp].wMask);
            //MSAUDIO_PRINT(" -----DePopSequence ==%d , ---uctemp ==%d-----dwReg ==%x ---wValue==%x    \r\n",DePopSequence++,uctemp,DEPOP_INIT_TBL[DePopSequence][uctemp].dwReg,DEPOP_INIT_TBL[DePopSequence][uctemp].wValue);
    }
    #if ENABLE_TIME_MEASUREMENT
        if (DePopSequence<DE_POP_Finish)
    		TIME_MEASURE_END();
    #endif    
	
    switch(DePopSequence)
    {
        case DE_POP_STEP1:
            MSAUDIO_PRINT("--- Audio De-Pop Step 1==%x\r\n",DAC_DELAY_STEP12);
			
            AudioDePopSequence = DE_POP_STEP2;
            bAudioDePopFlag = 0;
            SetDAC_DePopCounter(DAC_DELAY_STEP12);
            break;

        case DE_POP_STEP2:
            MSAUDIO_PRINT("--- Audio De-Pop Step 2==%x\r\n",DAC_DELAY_STEP23);
            AudioDePopSequence = DE_POP_STEP3;
            bAudioDePopFlag = 0;
            SetDAC_DePopCounter(DAC_DELAY_STEP23);
            break;

        case DE_POP_STEP3:
            MSAUDIO_PRINT("--- Audio De-Pop Step 3==%x\r\n",DAC_DELAY_STEP34);
            AudioDePopSequence = DE_POP_STEP4;
            bAudioDePopFlag = 0;
            SetDAC_DePopCounter(DAC_DELAY_STEP34);
            break;

        case DE_POP_STEP4:
            MSAUDIO_PRINT("--- Audio De-Pop Step 4==%x\r\n",DAC_DELAY_STEP45);
            AudioDePopSequence = DE_POP_STEP5;
            bAudioDePopFlag = 0;
            SetDAC_DePopCounter(DAC_DELAY_STEP45);
            break;

        case DE_POP_STEP5:
            MSAUDIO_PRINT("--- Audio De-Pop Step 5==%x\r\n",DAC_DELAY_STEP56);
            AudioDePopSequence = DE_POP_STEP6;
            bAudioDePopFlag = 0;
            SetDAC_DePopCounter(DAC_DELAY_STEP56);
            break;

        case DE_POP_STEP6:
            MSAUDIO_PRINT("--- Audio De-Pop Step 6==%x\r\n",DAC_DELAY_STEP67);
            AudioDePopSequence = DE_POP_STEP7;
            bAudioDePopFlag = 0;
            SetDAC_DePopCounter(DAC_DELAY_STEP67);
            break;

        case DE_POP_STEP7:
            MSAUDIO_PRINT("--- Audio De-Pop Step 7\r\n");
            //msDrvAudio_SetDPGAMuteSrc(E_DPGA_INIT_MUTE_OFF);
            AudioDePopSequence = DE_POP_Finish;
            bAudioDePopFlag = 0;
            SetDAC_DePopCounter(0);
            break;

        case DE_POP_Finish:
            MSAUDIO_PRINT("--- Audio DE_POP_Finish\r\n");
            break;

        default:
            //AudioDePopSequence = DE_POP_STEP1;
            //u16AudioDePopCounter = 0;
            break;
    }
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudioWakeUpDePop()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioWakeUpDePop(DAC_WKUP_STEP DePopSequence)
{
    BYTE uctemp=0;

    if (bAudioWakeUpDePopFlag == 0)
        return;

    for(uctemp = 0; uctemp <(sizeof(WakeUpDepopSequence_TBL)/sizeof(RegTbl2Type)/WKUP_DE_POP_Finish); uctemp++)
    {
            msWrite2ByteMask(WakeUpDepopSequence_TBL[DePopSequence][uctemp].dwReg, WakeUpDepopSequence_TBL[DePopSequence][uctemp].wValue ,WakeUpDepopSequence_TBL[DePopSequence][uctemp].wMask);
            MSAUDIO_PRINT(" -----Wake up DePopSequence ==%x , ---uctemp ==%x-----dwReg ==%x ---wValue==%x    \r\n",DePopSequence,uctemp,WakeUpDepopSequence_TBL[DePopSequence][uctemp].dwReg,WakeUpDepopSequence_TBL[DePopSequence][uctemp].wValue);
    }
    switch(DePopSequence)
    {
        case WKUP_DE_POP_STEP1:
            MSAUDIO_PRINT("--- Audio Wake Up De-Pop Step 1\r\n");
            AudioWKUPDePopSequence = WKUP_DE_POP_STEP2;
            bAudioWakeUpDePopFlag = 0;
            SetDAC_DePopCounter(WKUP_DELAY_STEP12);
            break;


        case WKUP_DE_POP_STEP2:
            MSAUDIO_PRINT("--- Audio Wake Up De-Pop Step 2\r\n");
            AudioWKUPDePopSequence = WKUP_DE_POP_STEP3;
            bAudioWakeUpDePopFlag = 0;
            SetDAC_DePopCounter(WKUP_DELAY_STEP23);
            break;

        case WKUP_DE_POP_STEP3:
            MSAUDIO_PRINT("--- Audio Wake Up De-Pop Step 3\r\n");
            AudioWKUPDePopSequence = WKUP_DE_POP_STEP4;
            bAudioWakeUpDePopFlag = 0;
            SetDAC_DePopCounter(WKUP_DELAY_STEP34);
            break;

        case WKUP_DE_POP_STEP4:
            MSAUDIO_PRINT("--- Audio Wake Up De-Pop Step 4\r\n");
            AudioWKUPDePopSequence = WKUP_DE_POP_STEP5;
            bAudioWakeUpDePopFlag = 0;
            SetDAC_DePopCounter(WKUP_DELAY_STEP45);
            break;

        case WKUP_DE_POP_STEP5:
            MSAUDIO_PRINT("--- Audio Wake Up De-Pop Step 5\r\n");
            AudioWKUPDePopSequence = WKUP_DE_POP_STEP6;
            bAudioWakeUpDePopFlag = 0;
            SetDAC_DePopCounter(WKUP_DELAY_STEP56);
            break;

        case WKUP_DE_POP_STEP6:
            MSAUDIO_PRINT("--- Audio Wake Up De-Pop Step 6\r\n");
            AudioWKUPDePopSequence = WKUP_DE_POP_STEP7;
            bAudioWakeUpDePopFlag = 0;
            SetDAC_DePopCounter(WKUP_DELAY_STEP67);
            break;

        case WKUP_DE_POP_STEP7:
            MSAUDIO_PRINT("--- Audio Wake Up De-Pop Step 7\r\n");
            AudioWKUPDePopSequence = WKUP_DE_POP_STEP8;
            bAudioWakeUpDePopFlag = 0;
            SetDAC_DePopCounter(WKUP_DELAY_STEP78);
            break;
				
        case WKUP_DE_POP_STEP8:
            MSAUDIO_PRINT("--- Audio Wake Up De-Pop Step 8\r\n");
            AudioWKUPDePopSequence = WKUP_DE_POP_Finish;
            bAudioWakeUpDePopFlag = 0;
            SetDAC_DePopCounter(0);
            break;

        case WKUP_DE_POP_Finish:
            MSAUDIO_PRINT("--- Audio Wake Up DE_POP_Finish\r\n");
            break;

        default:
            //AudioDePopSequence = WKUP_DE_POP_STEP1;
            //u16AudioDePopCounter = 0;
            break;
    }
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudioStandByDePop()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioStandbyDePop( void )
{
    BYTE uctemp1,uctemp2;
    for(uctemp1 = STDBY_DE_POP_STEP1; uctemp1 <STDBY_DE_POP_Finish; uctemp1++)
    {
        for(uctemp2 = 0; uctemp2 <(sizeof(StandByDepopSequence_TBL)/sizeof(RegTbl2Type)/STDBY_DE_POP_Finish); uctemp2++)	 
        {
            msWrite2ByteMask(StandByDepopSequence_TBL[uctemp1][uctemp2].dwReg, StandByDepopSequence_TBL[uctemp1][uctemp2].wValue ,StandByDepopSequence_TBL[uctemp1][uctemp2].wMask);
            MSAUDIO_PRINT(" -----StandBy Depop Sequence==%x , ---uctemp ==%x-----dwReg ==%x ---wValue==%x    \r\n",uctemp1,uctemp2,StandByDepopSequence_TBL[uctemp1][uctemp2].dwReg,StandByDepopSequence_TBL[uctemp1][uctemp2].wValue);
        }
    	}
}
#else
void msDrvAudioPoweronDACwithoutDePop(void)
{
    BYTE uctemp1,uctemp2;
    for(uctemp1 = DE_POP_STEP1; uctemp1 <DE_POP_Finish; uctemp1++)
    {
        for(uctemp2 = 0; uctemp2 <(sizeof(DEPOP_INIT_TBL)/sizeof(RegTbl2Type)/DE_POP_Finish); uctemp2++)
        {
                msWrite2ByteMask(DEPOP_INIT_TBL[uctemp1][uctemp2].dwReg, DEPOP_INIT_TBL[uctemp1][uctemp2].wValue,DEPOP_INIT_TBL[uctemp1][uctemp2].wMask);
                MSAUDIO_PRINT(" -----DePopSequence ==%x , ---uctemp ==%x-----dwReg ==%x ---wValue==%x    \r\n",uctemp1,uctemp2,DEPOP_INIT_TBL[uctemp1][uctemp2].dwReg,DEPOP_INIT_TBL[uctemp1][uctemp2].wValue);
        }
    }
}
#endif

//==============================================================================
// Audio INIT Relation
//==============================================================================
void msDrvAudioDACInit_DigitalPart(void)
{
    msWriteByte(REG_112C24, 0x08);  //tang for earphone noise

    msWriteByte(REG_112C2B, 0x1F);

    //enable clock from mpll
    msWriteByte(REG_101ECE, 0x18);
    msWriteByte(REG_101ECF, 0x00);
    msWriteByte(REG_101ECE, 0x10);
    msWriteByte(REG_101ECF, 0x00);

    msWriteByte(REG_000B00, 0x00);
    msWriteByte(REG_000B01, 0x00);

    //enable adc,dac,earphone driver
    msWriteByte(REG_112CDA, 0x22);
    msWriteByte(REG_112CDB, 0x7E);

    //enable inmux,reference
    msWriteByte(REG_112CDC, 0x18);
    msWriteByte(REG_112CDD, 0x00);

    //select inmux input and gain
    msWriteByte(REG_112CE0, 0x00);
    msWriteByte(REG_112CE1, 0x00);

    //select ausdm_atop clock, enable chop and dither
    msWriteByte(REG_112CE2, 0x00);
    msWriteByte(REG_112CE3, 0x0e);

    msWriteByte(REG_112CE4, 0x00);
    msWriteByte(REG_112CE5, 0x00);

    msWriteByte(REG_112CE6, 0x00);
    msWriteByte(REG_112CE7, 0x00);

    msWriteByte(REG_112C2A, 0x1F);      //[4:0] REG_DC_OFFSET_EN 
#if (MS_BOARD_TYPE_SEL== BD_MST9UHL_V4||MS_BOARD_TYPE_SEL==BD_MST9UHL_V6||MS_BOARD_TYPE_SEL==BD_MST9UHL_V7||MS_BOARD_TYPE_SEL==BD_MST9UHL_V4_1H1DP||MS_BOARD_TYPE_SEL==BD_MST9UHD_BV1||MS_BOARD_TYPE_SEL==BD_MST9UHD_DV1) 
    msWrite2Byte(REG_112C2C, 0x0D0);      //Hearphone LR Swap 
#else
    msWrite2Byte(REG_112C2C, 0x1D0);      //Hearphone LR Swap 
#endif

    msWrite2Byte(REG_112C2E, 0x80);     //[15:0] REG_DC_OFFSET[15:0]

    msWrite2Byte(REG_112C20, 0x38C0);   //[15:8] REG_BOUNDARY1_SEL  [7:0] REG_BOUNDARY0_SEL    THRD to protect gain is overflow due to dither
    msWrite2Byte(REG_112C22, 0x3838);   //[15:8] REG_BOUNDARY3_SEL [7:0] REG_BOUNDARY2_SELTHRD to protect gain is overflow due to dither

    msWriteByte(REG_112C2B, 0x3F);  // Dither enable 
    msWriteByte(REG_112C32, 0x20);  // Set dither x 0.5

    
    msWrite2ByteMask( REG_112C00, BIT13    , 0xFFFF);  //Backend reset
    ForceDelay1ms(2);
    msWrite2ByteMask( REG_112C00, BIT15    , 0xFFFF);  //toggle reset, RD said Bit15 is used to reset all DAC part , dont need to reset each sub-part.Reset Bit13 still can hear "po" sound
    ForceDelay1ms(2);
    msWrite2ByteMask(  REG_112C00, BIT0    , 0xFFFF);  // [0] REG_AU_RESET_MASK : mask AU_RESET from SDM/DWA RESET (BACKEND_RESET) 0=mask, 1=normal

    msWrite2ByteMask(  REG_112C06, 0xFF00    , 0xFFFF);  //clock enable
    msWrite2ByteMask(  REG_112C46, 0x8000    , 0xFFFF);  //enable initial audioband's sram
    ForceDelay1ms(1);
    msWrite2ByteMask(  REG_112C46, 0x0000    , 0xFFFF);  //disable initial audioband's sram
    msWrite2ByteMask(  REG_112C46, 0x0600    , 0xFFFF);  //enable SDM/DWA  [10] REG_SDM_DEMO_EN :enable SDM ,[9] REG_SDM_2ND_ORDER :enable 2nd order SDM


#if AUDIDO_ADC_HPF_N
    msWrite2ByteMask(  REG_112C74, 0x00F0    , 0xFFFF);   //enable high pass filter:
#else
    msWrite2ByteMask(  REG_112C74, 0x0000    , 0xFFFF);   //bypass high pass filter
#endif

    msWrite2ByteMask(  REG_112CA4, 0x0073    , 0xFF7F);     //[15:8] DPGA_1_TIMEOUT_N; [6:4] DPGA_1_STEP=3'd7->N=1;  [2] DPGA_1_ZERO_DET_EN; [1] DPGA_1_FADING_EN;   [0] DPGA_1_DPGA_EN;

    msWriteByteMask(  REG_112C81, 0x00  , BIT7);    //[11]:set to I2S justified mode,

}

void msDrvAudioDACInit_AnalogPart(void)
{
    msWrite2ByteMask( REG_112CDA, BIT9|BIT5|BIT4        , BIT15|BIT14|BIT11|BIT9|BIT7|BIT5|BIT4|BIT1);
    msWrite2ByteMask( REG_112CE2, /*BIT5|*/BIT2         , BIT15|BIT14|BIT11|BIT10|BIT9|BIT8|BIT7|BIT6|BIT5|BIT4|BIT3|BIT2);
    msWrite2ByteMask( REG_112CEA, BIT9                 , BIT9);
	
//    msWrite2ByteMask( REG_112CE4, BIT14                 , BIT15|BIT14|BIT13);
}

#if ENABLE_I2S || ENABLE_SPDIF
void msAudio_I2S_SPDIF_Init(void)
{
    //pad setting
    #if ENABLE_SPDIF
       Init_SPIFGPIO_Pin(); //SPDIF -> PAD
    #endif
    #if ENABLE_I2S 
        Init_I2S_Switch_Pin();
    #endif

    #if ENABLE_I2S_PassThroughDAC
	msWriteByteMask(  REG_112C81, BIT6  , BIT6);    //[11]:I2S BCK invert function   //<<== according external DAC's format to set

	msWrite2ByteMask( REG_112C04, 	BIT6/*|BIT4*/|0x08 ,	BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0 ); //[6] test clock enable [5:4] MCLK divider [3:0] BCLK divider
    #endif
    #if ENABLE_I2S_FIX_MCLK
    msWriteByteMask(REG_112C04, 0x63, 0x7F);        // fix MCLK from haydn ~13M
    msWriteByteMask(REG_000407, 0x40, 0x42);        // pad switch to mclk au
    #endif
}
#endif

void msDrvAudioDACInit( void )
{
    //MSAUDIO_PRINT("...------------------------------ REG_00406..===%x\r\n",msReadByte(REG_000406));
    //MSAUDIO_PRINT("..------------------------------ REG_00407..===%x\r\n",msReadByte(REG_000407));
    #if ENABLE_I2S || ENABLE_SPDIF
    msAudio_I2S_SPDIF_Init();
    #endif
    msDrvAudioDACInit_DigitalPart();
    msDrvAudioDACInit_AnalogPart();
    #if ENABLE_TIME_MEASUREMENT
        TIME_MEASURE_BEGIN();
    #endif			
	
    #if EnableDACDePop
        if( (g_eBootStatus == eBOOT_STATUS_DCON) || (g_eBootStatus == eBOOT_STATUS_DPMSON))
        {
            bAudioWakeUpDePopFlag=1;
            msDrvAudioWakeUpDePop(WKUP_DE_POP_STEP1);
        }
        else
        {
            bAudioDePopFlag=1;
            msDrvAudioPoweronDePop(DE_POP_STEP1);
    }	 
    #else
        msDrvAudioDACInit_DigitalPart();
        msDrvAudioDACInit_AnalogPart();
        msDrvAudioPoweronDACwithoutDePop();
    #endif
    MSAUDIO_PRINT("... Audio Initialize .777..\r\n");
}
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudioSetInit()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioSetInit( void )
{
#if EnableDACDePop
        bAudioDePopFlag = 0;
        bAudioWakeUpDePopFlag = 0;
#endif
        m_bDPGA_InitAudioMute           = FALSE;
        m_bDPGA_SourceChangeAudioMute   = FALSE;
        m_bDPGA_MenuAudioMute           = FALSE;
        m_bDPGA_AVSyncAudioMute         = FALSE;
        m_bDPGA_TimingChangeAudioMute   = FALSE;
        m_bDPGA_ModeChangeAudioMute     = FALSE;
        m_bDPGA_DelayChangeAudioMute     = FALSE;
        m_bDPGA_Internal1AudioMute      = FALSE;

        m_bSW_MailboxAudioMute          = FALSE;
        m_bSW_SourceChangeAudioMute     = FALSE;
        m_bSW_InitAudioMute             = FALSE;
        m_bSW_MenuAudioMute             = FALSE;
        m_bSW_AVSyncAudioMute           = FALSE;
        m_bSW_TimingChangeAudioMute     = FALSE;
        m_bSW_ModeChangeAudioMute       = FALSE;
        m_bSW_Internal1AudioMute        = FALSE;

}


//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAudioDPGA_SetVolume()
/// @brief \b Function \b Description: Set Audio DPGA Volume
/// @param <IN>        \b eDPGA: DPGA channle,
///                    \b        mapping -> DPGA0:DAC0, DPGA1:DAC1, DPGA2:DAC2m DPGA3:DAC3
/// @param <IN>        \b eChSelect: Channel selection
/// @param <IN>        \b Volume_Integer: Volume Integer Part from 0 ~ -113dB
/// @param <IN>        \b Volume_Fraction: Volume Fraction Part, -0.125dB per unit
///                    \b        sel in AUDIO_VOLUME_FRACTION_PART
/// @return <OUT>      \b
//------------------------------------------------------------------------------
#define Audio_Out_New               0//((defined(LianHeChuangXin_Project)))


#if ENABLE_AUDIO_NEW_CURVE

#define MAX_EARPHONE_VOLUME     0x120
#define MIN_EARPHONE_VOLUME     0x00

#define MAX_EARPHONE_VOLUME_2     0x89
#define MIN_EARPHONE_VOLUME_2     0x00


#else

#if (defined(HKC_Project))
#define MAX_EARPHONE_VOLUME  0x7FF  //  0x750
#define DEF_EARPHONE_VOLUME_H 0x7C0    // 0x700

#define DEF_EARPHONE_VOLUME_L      0x00
#define MIN_EARPHONE_VOLUME      0x90

#elif (defined(LeXingEnMa_Project))
#if 0
#define MAX_EARPHONE_VOLUME     0x700
#define DEF_EARPHONE_VOLUME     0x500
#define MIN_EARPHONE_VOLUME     0x80
#else
#define MAX_EARPHONE_VOLUME     0x180
#define DEF_EARPHONE_VOLUME     0xC0//0x50//0xD0
#define MIN_EARPHONE_VOLUME     0x00
#endif

#elif (defined(JieBo_Project)||defined(JieBo_BangZi_Project))
#define MAX_EARPHONE_VOLUME     0x180
#define DEF_EARPHONE_VOLUME     0x70//0x88//0x50//0xD0
#define MIN_EARPHONE_VOLUME     0x13
#elif (defined(ZDCC_Project))
#define MAX_EARPHONE_VOLUME     0xD0
#define DEF_EARPHONE_VOLUME     0x88//0x50//0xD0
#define MIN_EARPHONE_VOLUME     0x28

#elif (defined(KunDe_Project))
#define MAX_EARPHONE_VOLUME     0xA0//0x90
#define DEF_EARPHONE_VOLUME     0x70//0x80
#define MIN_EARPHONE_VOLUME     0x56//0x70

#elif (defined(ZhouYang_Project))
#define MAX_EARPHONE_VOLUME     0xD0//0x90
#define DEF_EARPHONE_VOLUME     0x80
#define MIN_EARPHONE_VOLUME     0x70

#elif (defined(HuiGe_Project))
#define MAX_EARPHONE_VOLUME     0xD0//0x90
#define DEF_EARPHONE_VOLUME      0x66//0x80
#define MIN_EARPHONE_VOLUME      0x00

#define MAX_EARPHONE_VOLUME_A     0xD0//0x90
#define DEF_EARPHONE_VOLUME_A      0x50//0x80
#define MIN_EARPHONE_VOLUME_A      0x00//0x15//0x00

#elif (defined(HuaKe_Project))

#define MAX_EARPHONE_VOLUME     0xF0
#define DEF_EARPHONE_VOLUME     0x70//0x50//0xD0
#define MIN_EARPHONE_VOLUME     0x00

#elif (defined(FengYuan_Project))
#if (PanelType==PanelLSC400FN04)
#define MAX_EARPHONE_VOLUME     0x180
#define DEF_EARPHONE_VOLUME     0xC6//0x90//0x50//0xD0
#define MIN_EARPHONE_VOLUME     0x8E//0x6E


#define MAX_EARPHONE_VOLUME_LINEOUT     0x130
#define DEF_EARPHONE_VOLUME_LINEOUT     0x90
#define MIN_EARPHONE_VOLUME_LINEOUT     0x00

#elif (defined(Japan_Project))
#define MAX_EARPHONE_VOLUME     0x180
#define DEF_EARPHONE_VOLUME     0x64//0x90//0x50//0xD0
#define MIN_EARPHONE_VOLUME     0x39//0x00
#else
#if 0//8ohm 2W
#define MAX_EARPHONE_VOLUME     0x180
#define DEF_EARPHONE_VOLUME      0xC0//0x50//0xD0
#define MIN_EARPHONE_VOLUME      0x6B
#else

#define MAX_EARPHONE_VOLUME     0x180
#define DEF_EARPHONE_VOLUME     0x90//0x50//0xD0
#define MIN_EARPHONE_VOLUME     0x00
#endif
#endif

#elif (defined(Kogan_Project))
#define MAX_EARPHONE_VOLUME     0x180
#define DEF_EARPHONE_VOLUME     0x88//0x50//0xD0
#define MIN_EARPHONE_VOLUME     0x50//0x13

#elif (defined(LianHeChuangXin_Project))
  #if Audio_Out_New
  #define MAX_EARPHONE_VOLUME  0x7FF  
  #define DEF_EARPHONE_VOLUME_H 0x7E6//*OSD 70*///0x7F3/*OSD 60*///0x7C0   
  
  #define DEF_EARPHONE_VOLUME_L      0x00
  #define MIN_EARPHONE_VOLUME      0x90

  #else
  #define MAX_EARPHONE_VOLUME     0x160
  #define DEF_EARPHONE_VOLUME     0x40
  #define MIN_EARPHONE_VOLUME     0x14//0x00
  #endif
#elif (defined(ZhongXinKeJi_Project))  
  #define MAX_EARPHONE_VOLUME     0xD0
  #define DEF_EARPHONE_VOLUME     0x40
  #define MIN_EARPHONE_VOLUME     0x00

#elif (defined(K3L_CrossOver_Project))  
#define MAX_EARPHONE_VOLUME        0x7EA//0x7FF  //  0x750
#define DEF_EARPHONE_VOLUME_H    0x7B0    // 0x700

#define DEF_EARPHONE_VOLUME_L    0x00
#define MIN_EARPHONE_VOLUME        0x90

#elif defined(KTC_Project)
#define MAX_EARPHONE_VOLUME_LINEOUT     0x150
#define DEF_EARPHONE_VOLUME_LINEOUT     0x90
#define MIN_EARPHONE_VOLUME_LINEOUT     0x00


#define MAX_EARPHONE_VOLUME  0xc0  //  0x750
#define DEF_EARPHONE_VOLUME_H 0xb0    // 0x700

#define DEF_EARPHONE_VOLUME_L      0x00
#define MIN_EARPHONE_VOLUME      0x90
#elif (defined(XuLi_Project))//PanelType==PanelLSF490FJ02
#define MAX_EARPHONE_VOLUME     0x150//0xD0
#define DEF_EARPHONE_VOLUME     0x40
#define MIN_EARPHONE_VOLUME     0x09
#elif (defined(MingCai_Project))
#define MAX_EARPHONE_VOLUME     0xD0
#define DEF_EARPHONE_VOLUME     0x40
#define MIN_EARPHONE_VOLUME     0x09
#elif 0//((defined(LianHeChuangXin_Project))&&(Audio_Out_New))
#define MAX_EARPHONE_VOLUME  0x7FF  
#define DEF_EARPHONE_VOLUME_H 0x7E6//*OSD 70*///0x7F3/*OSD 60*///0x7C0   

#define DEF_EARPHONE_VOLUME_L      0x00
#define MIN_EARPHONE_VOLUME      0x90

#else
#define MAX_EARPHONE_VOLUME  0x7FF  //  0x750
#define DEF_EARPHONE_VOLUME_H 0x7C0    // 0x700

#define DEF_EARPHONE_VOLUME_L      0x00
#define MIN_EARPHONE_VOLUME      0x90
#endif


#endif

#ifndef MAX_EARPHONE_VOLUME_A
#define MAX_EARPHONE_VOLUME_A     MAX_EARPHONE_VOLUME
#endif

#ifndef DEF_EARPHONE_VOLUME_A
#define DEF_EARPHONE_VOLUME_A     DEF_EARPHONE_VOLUME
#endif

#ifndef MIN_EARPHONE_VOLUME_A
#define MIN_EARPHONE_VOLUME_A     MIN_EARPHONE_VOLUME
#endif

#ifndef MAX_EARPHONE_VOLUME_LINEOUT
#define MAX_EARPHONE_VOLUME_LINEOUT     MAX_EARPHONE_VOLUME
#endif

#ifndef DEF_EARPHONE_VOLUME_LINEOUT
#define DEF_EARPHONE_VOLUME_LINEOUT     DEF_EARPHONE_VOLUME
#endif

#ifndef MIN_EARPHONE_VOLUME_LINEOUT
#define MIN_EARPHONE_VOLUME_LINEOUT     MIN_EARPHONE_VOLUME
#endif

#if ENABLE_AUDIO_NEW_CURVE
#define DefalutVolume         DEF_VOLUME
#else
#define DefalutVolume         50
#endif

void msDrvAudioDPGASetVolume( AUDIO_DPGA_SEL eDPGA, AUDIO_CH_SEL eChSelect, signed short iVolume_Integer, AUDIO_VOLUME_FRACTION_PART Volume_Fraction )
{
    BYTE u8Offset = eDPGA*8;
#if !AudioNewCurve	
    U8   voltab_index;
#endif
    U16  reg16_LCHVolume,reg16_RCHVolume;


#if AudioNewCurve


      //printData("iVolume_Integer==%d",iVolume_Integer);

#if ENABLE_AUDIO_NEW_CURVE


     if ( iVolume_Integer <= DefalutVolume )
        {
            reg16_LCHVolume = MAX_EARPHONE_VOLUME - ( ( DWORD ) ( MAX_EARPHONE_VOLUME - MIN_EARPHONE_VOLUME ) *  (iVolume_Integer ) ) / (DefalutVolume);
        }
        else
        {
           reg16_LCHVolume = MAX_EARPHONE_VOLUME_2- ( ( DWORD ) ( MAX_EARPHONE_VOLUME_2 - MIN_EARPHONE_VOLUME_2 ) *  ( iVolume_Integer - DefalutVolume ) ) / (100-DefalutVolume);
       }

#elif (defined(LeXingEnMa_Project)||(defined(ZhongXinKeJi_Project))||defined(JieBo_Project)||defined(MingCai_Project)||defined(Kogan_Project)||defined(KunDe_Project)||(defined(ZDCC_Project))||(defined(ZhouYang_Project))||(defined(JieBo_BangZi_Project))||(defined(HuiGe_Project))||(defined(HuaKe_Project))||(defined(FengYuan_Project))||(defined(DaYu_Project))||((defined(LianHeChuangXin_Project))&&(!Audio_Out_New))||(defined(XuLi_Project)))

          iVolume_Integer=(100-iVolume_Integer);

#if ENABLE_ADJ_AUDIO_OUTPUT
 if(USER_PREF_AUDIO_OUTPUTTYPE!=Speaker)//for SPIDF&Earphonne Volume
  {
     if ( iVolume_Integer > DefalutVolume )
        {
            reg16_LCHVolume = DEF_EARPHONE_VOLUME_LINEOUT + ( ( DWORD ) ( MAX_EARPHONE_VOLUME_LINEOUT - DEF_EARPHONE_VOLUME_LINEOUT ) * ( iVolume_Integer - DefalutVolume ) ) / (100-DefalutVolume);
        }
        else
        {
            reg16_LCHVolume = MIN_EARPHONE_VOLUME_LINEOUT + ( ( DWORD ) ( DEF_EARPHONE_VOLUME_LINEOUT - MIN_EARPHONE_VOLUME_LINEOUT ) * ( iVolume_Integer ) ) / (DefalutVolume);
        }
   }
  else
#endif
if(
   #if ENABLE_VGA_INPUT	
	CURRENT_INPUT_IS_ANALOG1()
   #else
       0
   #endif	
   #if ENABLE_DVI_INPUT
	 ||CURRENT_INPUT_IS_DIGITAL_DVI()
	#if ENABLE_DVI2ND_INPUT 
	 ||CURRENT_INPUT_IS_DIGITAL_DVI2ND()
	#endif 
 	#if ENABLE_DVI3ND_INPUT 
	 ||CURRENT_INPUT_IS_DIGITAL_DVI3ND()
	#endif 
   #endif
      )
   {
     if ( iVolume_Integer > DefalutVolume )
        {
            reg16_LCHVolume = DEF_EARPHONE_VOLUME_A + ( ( DWORD ) ( MAX_EARPHONE_VOLUME_A - DEF_EARPHONE_VOLUME_A ) * ( iVolume_Integer - DefalutVolume ) ) / (100-DefalutVolume);
        }
        else
        {
            reg16_LCHVolume = MIN_EARPHONE_VOLUME_A + ( ( DWORD ) ( DEF_EARPHONE_VOLUME_A - MIN_EARPHONE_VOLUME_A ) * ( iVolume_Integer ) ) / (DefalutVolume);
        }

   }
   else
   {
     if ( iVolume_Integer > DefalutVolume )
        {
            reg16_LCHVolume = DEF_EARPHONE_VOLUME + ( ( DWORD ) ( MAX_EARPHONE_VOLUME - DEF_EARPHONE_VOLUME ) * ( iVolume_Integer - DefalutVolume ) ) / (100-DefalutVolume);
        }
        else
        {
            reg16_LCHVolume = MIN_EARPHONE_VOLUME + ( ( DWORD ) ( DEF_EARPHONE_VOLUME - MIN_EARPHONE_VOLUME ) * ( iVolume_Integer ) ) / (DefalutVolume);
        }
   }
#elif (defined(HKC_Project))
      if ( iVolume_Integer > DefalutVolume )
        {
            reg16_LCHVolume = MAX_EARPHONE_VOLUME - ( ( DWORD ) ( MAX_EARPHONE_VOLUME - DEF_EARPHONE_VOLUME_H ) * ( iVolume_Integer - DefalutVolume ) ) / (100-DefalutVolume);
        }
        else
        {
            reg16_LCHVolume = DEF_EARPHONE_VOLUME_L + ( ( DWORD ) ( MIN_EARPHONE_VOLUME - 0 ) * ( DefalutVolume-iVolume_Integer ) ) / (DefalutVolume);
        }
#elif (defined(KTC_Project))
          iVolume_Integer=(100-iVolume_Integer);

     if ( iVolume_Integer > DefalutVolume )
        {
            reg16_LCHVolume = DEF_EARPHONE_VOLUME_LINEOUT + ( ( DWORD ) ( MAX_EARPHONE_VOLUME_LINEOUT - DEF_EARPHONE_VOLUME_LINEOUT ) * ( iVolume_Integer - DefalutVolume ) ) / (100-DefalutVolume);
        }
        else
        {
            reg16_LCHVolume = MIN_EARPHONE_VOLUME_LINEOUT + ( ( DWORD ) ( DEF_EARPHONE_VOLUME_LINEOUT - MIN_EARPHONE_VOLUME_LINEOUT ) * ( iVolume_Integer ) ) / (DefalutVolume);
        }

		
#else

      if ( iVolume_Integer > DefalutVolume )
        {
            reg16_LCHVolume = MAX_EARPHONE_VOLUME - ( ( DWORD ) ( MAX_EARPHONE_VOLUME - DEF_EARPHONE_VOLUME_H ) * ( iVolume_Integer - DefalutVolume ) ) / (100-DefalutVolume);
        }
        else
        {
            reg16_LCHVolume = DEF_EARPHONE_VOLUME_L + ( ( DWORD ) ( MIN_EARPHONE_VOLUME - 0 ) * ( DefalutVolume-iVolume_Integer ) ) / (DefalutVolume);
        }

#endif

      reg16_RCHVolume=reg16_LCHVolume;

      Volume_Fraction=Volume_Fraction;
	  
      //printData("reg16_LCHVolume==%x",reg16_LCHVolume);
#else
    if (iVolume_Integer > 0)                         // (+911) * -0.125dB = -113.875dB
    {
        iVolume_Integer = 0;                         // maximum is 0dB
    }
    else if (iVolume_Integer <= -113)
    {
        iVolume_Integer = -113;                      //minimum
    }

    voltab_index = abs(iVolume_Integer);
    reg16_RCHVolume = AudVolumeTab[voltab_index]+Volume_Fraction;
    reg16_LCHVolume=((reg16_RCHVolume+5)>911)?(reg16_RCHVolume):(reg16_RCHVolume+5);  //-96~-1(0x7A0~0x7FF) still satify this condition.
#endif
    #if 0
        MSAUDIO_PRINT("--->1. volume_Integer=%d, voltab_index= %d\r\n",iVolume_Integer, voltab_index);
        MSAUDIO_PRINT("--->2. Vol Ing =%x, Vol Fra=%x\r\n",AudVolumeTab[voltab_index],Volume_Fraction);
        MSAUDIO_PRINT("--->3. L Channel Register Vol=%x\r\n",reg16_LCHVolume);
        MSAUDIO_PRINT("--->4. R Channel Register Vol=%x\r\n",reg16_RCHVolume);
	
    #endif

    switch(eDPGA)
    {
        case AUDIO_DPGA0:
            if (eChSelect == E_AUDIO_LEFT_CH)
            {
                dpga0_volume_lt_store = reg16_LCHVolume;
            }
            else if (E_AUDIO_RIGHT_CH)
            {
                dpga0_volume_rt_store = reg16_RCHVolume;
            }
            else
            {
                dpga0_volume_lt_store = reg16_LCHVolume;
                dpga0_volume_rt_store = reg16_RCHVolume;
            }
            break;


        case AUDIO_DPGA_ALL:
        default:
                dpga0_volume_lt_store = reg16_LCHVolume;
                dpga0_volume_rt_store = reg16_RCHVolume;
            break;
    }

    if( eDPGA == AUDIO_DPGA_ALL )
    {
        eChSelect = E_AUDIO_LEFT_RIGHT_CH;
        u8Offset = 0;
    }
    else
    {
        //msWriteByteMask(REG_140300, 0x00, BIT5);
        u8Offset = 0;  //only one DPGA,
    }

    switch(eChSelect)
    {
        case E_AUDIO_LEFT_CH:
            msWrite2ByteMask(REG_112CA6 + u8Offset,  reg16_LCHVolume    ,0x07FF);
            break;

        case E_AUDIO_RIGHT_CH:
            msWrite2ByteMask(REG_112CA8 + u8Offset, reg16_RCHVolume , 0x07FF);
            break;

        case E_AUDIO_LEFT_RIGHT_CH:
            if( eDPGA == AUDIO_DPGA_ALL )
            {
                msWrite2ByteMask(REG_112CA6, reg16_LCHVolume, 0x07FF);
                msWrite2ByteMask(REG_112CA8, reg16_RCHVolume, 0x07FF);
            }
            else
            {
                msWrite2ByteMask(REG_112CA6 + u8Offset, reg16_LCHVolume, 0x07FF);
                msWrite2ByteMask(REG_112CA8 + u8Offset, reg16_RCHVolume, 0x07FF);
            }
            break;
        default:
            break;
    }
    if( eDPGA == AUDIO_DPGA_ALL )
    {
        msWriteByteMask(REG_112CA4, 0x00, BIT7);
        msWriteByteMask(REG_112CA4, BIT7, BIT7);
   #if ENABLE_AUDIO_NEW_CURVE	
      if(USER_PREF_VOLUME<=DefalutVolume)
  	   msWrite2ByteMask(REG_112CE8, 0x0000, 0x0001);
      else
  	   msWrite2ByteMask(REG_112CE8, 0x0001, 0x0001);	  	
   #endif
        msWriteByteMask(REG_112CA4, 0x00, BIT7);
    }
    else
    {
        msWriteByteMask(REG_112CA4 + u8Offset, 0x00, BIT7);
        msWriteByteMask(REG_112CA4 + u8Offset, BIT7, BIT7);
        msWriteByteMask(REG_112CA4 + u8Offset, 0x00, BIT7);
    }
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudioDPGA_SetVolRegMute(eDPGA)
/// @brief \b Function \b Description: Store DPGA gain
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioDPGA_SetVolRegMute(void)
{
        msWrite2ByteMask(REG_112CA6, 0x038f, 0x07FF);   //[10:0] DPGA_1_GAIN_L
        msWrite2ByteMask(REG_112CA8, 0x038f, 0x07FF);   //[10:0] DPGA_1_GAIN_R
        //[0] DPGA_1_DPGA_EN    ,[1] DPGA_1_FADING_EN  ,[2] DPGA_1_ZERO_DET_EN ,[6:4] DPGA_1_STEP ,[7] DPGA_1_GAIN_TRIG   ,[15:8] DPGA_1_TIMEOUT_N
        msWrite2ByteMask(REG_112CA4, 0x00|BIT2|BIT0, BIT7|BIT2|BIT0);
        msWrite2ByteMask(REG_112CA4, BIT7|BIT2|BIT0, BIT7|BIT2|BIT0);
        msWrite2ByteMask(REG_112CA4, 0x00|BIT2|BIT0, BIT7|BIT2|BIT0);
		#if AUDIO_MUTE_DEBUG
        MSAUDIO_PRINT("@@@@@@@@@@@> msDrvAudioDPGA_SetVolRegMute,  \r\n", 0);
		#endif
        audio_dpga_mute_flag = 1;
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAudioDPGA_StoreVolume(eDPGA)
/// @brief \b Function \b Description: Store DPGA gain
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioDPGA_RecoverVolume(void)
{
    if (audio_dpga_mute_flag)
    {
        msWrite2ByteMask(REG_112CA6, dpga0_volume_lt_store, 0x07FF);    //[10:0] DPGA_1_GAIN_L
        msWrite2ByteMask(REG_112CA8, dpga0_volume_rt_store, 0x07FF);    //[10:0] DPGA_1_GAIN_R
        //[0] DPGA_1_DPGA_EN    ,[1] DPGA_1_FADING_EN  ,[2] DPGA_1_ZERO_DET_EN ,[6:4] DPGA_1_STEP ,[7] DPGA_1_GAIN_TRIG   ,[15:8] DPGA_1_TIMEOUT_N
        msWrite2ByteMask(REG_112CA4, 0x00|BIT2|BIT0, BIT7|BIT2|BIT0);
        msWrite2ByteMask(REG_112CA4, BIT7|BIT2|BIT0, BIT7|BIT2|BIT0);
        msWrite2ByteMask(REG_112CA4, 0x00|BIT2|BIT0, BIT7|BIT2|BIT0);
		#if AUDIO_MUTE_DEBUG
        MSAUDIO_PRINT("@@@@@@@@@@@> msDrvAudioDPGA_RecoverVolume,  \r\n", 0);
		#endif
        audio_dpga_mute_flag = 0;
    }
}


//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudio_SetEarphoneMute()
/// @brief \b Function \b Description: Earphone Mute.
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudio_SetEarphoneMute(MS_BOOL enable)
{
    //[ 0   ]EN_STG2_AB_EAR :Earphone output strong driving
    //[ 1   ]EN_STG2_LP_EAR  :Earphone output weak driving
    //[ 2   ]EN_LTX_EAR :Earphone L-CH weak driving output
    //[ 3   ]EN_RTX_EAR :Earphone R-CH weak driving output
    //[ 4   ]EN_MUTE_EAR :Earphone output mute
    //[ 5   ]EN_BYP_INMUX0  :Audio INMUX bypass enable
    //[ 7   ]EN_MUTE_INMUX0  :Audio INMUX MUTE enable
    //[ 9   ]EN_VREF_DISCH   :Audio VREF generation block buffer output (VAG, VRP) node discharge control
    //[14:10]EN_VREF_SFTDCH :Audio VREF block soft discharge enable
    if (enable)
    {
        msWrite2ByteMask(REG_112CE8, 0x0000, 0x0001);
        Delay1ms(20);
        msWrite2ByteMask(REG_112CE8, 0x0000, 0x000C);
        Delay1ms(20);
        msWrite2ByteMask(REG_112CE8, 0x0000, 0x0002);
        //Delay1ms(25);        
        //msWrite2ByteMask(REG_112CE8, 0x0010, 0x0010);
    }
    else
    {    
        msWrite2ByteMask(REG_112CE8, 0x0002, 0x0002);
        Delay1ms(25);
        msWrite2ByteMask(REG_112CE8, 0x0000, 0x0010);
        Delay1ms(25);
        msWrite2ByteMask(REG_112CE8, 0x000C, 0x000C);
        Delay1ms(25);      
        msWrite2ByteMask(REG_112CE8, 0x0001, 0x0001);
    }	 

}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudio_SetDPGAMuteSrc()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudio_SetDPGAMuteSrc(AUDIO_MUTE_SOURCE_SEL mute_src_type)
{
        MS_BOOL SpkrMute;

        switch(mute_src_type)
        {
            case E_DPGA_INIT_MUTE_OFF:
                m_bDPGA_InitAudioMute = FALSE;
                break;

            case E_DPGA_INIT_MUTE_ON:
                m_bDPGA_InitAudioMute = TRUE;
                break;

            case E_DPGA_SOURCE_CHANGE_MUTE_OFF:
                m_bDPGA_SourceChangeAudioMute = FALSE;
                break;

            case E_DPGA_SOURCE_CHANGE_MUTE_ON:
                m_bDPGA_SourceChangeAudioMute = TRUE;
                break;

            case E_DPGA_MENU_MUTE_OFF:
                m_bDPGA_MenuAudioMute = FALSE;
                break;

            case E_DPGA_MENU_MUTE_ON:
                m_bDPGA_MenuAudioMute = TRUE;
                break;

            case E_DPGA_AVSYNC_MUTE_OFF:
                m_bDPGA_AVSyncAudioMute = FALSE;
                break;

            case E_DPGA_AVSYNC_MUTE_ON:
                m_bDPGA_AVSyncAudioMute = TRUE;
                break;

            case E_DPGA_TIMING_CHANGE_MUTE_OFF:
                m_bDPGA_TimingChangeAudioMute = FALSE;
                break;

            case E_DPGA_TIMING_CHANGE_MUTE_ON:
                m_bDPGA_TimingChangeAudioMute = TRUE;
                break;

            case E_DPGA_MODE_CHANGE_MUTE_OFF:
                m_bDPGA_ModeChangeAudioMute = FALSE;
                break;

            case E_DPGA_MODE_CHANGE_MUTE_ON:
                m_bDPGA_ModeChangeAudioMute = TRUE;
                break;

            case E_DPGA_DELAY_CHANGE_MUTE_OFF:
                m_bDPGA_DelayChangeAudioMute = FALSE;
                break;

            case E_DPGA_DELAY_CHANGE_MUTE_ON:
                m_bDPGA_DelayChangeAudioMute = TRUE;
                break;

            case E_DPGA_INTERNAL_1_MUTE_OFF:
                m_bDPGA_Internal1AudioMute = FALSE;
                break;

            case E_DPGA_INTERNAL_1_MUTE_ON:
                m_bDPGA_Internal1AudioMute = TRUE;
                break;

            default:
                  break;
        }

    #if (AUDIO_MUTE_DEBUG)
        MSAUDIO_PRINT("--> audio DPGA mute status: p:%d,  \r\n", m_bDPGA_InitAudioMute);
        MSAUDIO_PRINT("sr:%d,  \r\n",m_bDPGA_SourceChangeAudioMute);
        MSAUDIO_PRINT("me:%d,  \r\n",m_bDPGA_MenuAudioMute);
        MSAUDIO_PRINT("av:%d,  \r\n",m_bDPGA_AVSyncAudioMute);
        MSAUDIO_PRINT("tm:%d,  \r\n",m_bDPGA_TimingChangeAudioMute);
        MSAUDIO_PRINT("mo:%d,  \r\n",m_bDPGA_ModeChangeAudioMute);
        MSAUDIO_PRINT("de:%d,  \r\n",m_bDPGA_DelayChangeAudioMute);
        MSAUDIO_PRINT("i1:%d,  \r\n",m_bDPGA_Internal1AudioMute);
    #endif

        SpkrMute =  m_bDPGA_InitAudioMute|
                    m_bDPGA_SourceChangeAudioMute|
                    m_bDPGA_MenuAudioMute|
                    m_bDPGA_AVSyncAudioMute|
                    m_bDPGA_TimingChangeAudioMute|
                    m_bDPGA_ModeChangeAudioMute|
                    m_bDPGA_DelayChangeAudioMute|
                    m_bDPGA_Internal1AudioMute;


        if(SpkrMute == TRUE)
        {
            msDrvAudioDPGA_SetVolRegMute();
			#if AUDIO_MUTE_DEBUG
            MSAUDIO_PRINT("===> audio msDrvAudioDPGA_SetVolRegMute: p:%d,  \r\n", SpkrMute);
			#endif
        }
        else
        {
            msDrvAudioDPGA_RecoverVolume();
			#if AUDIO_MUTE_DEBUG
            MSAUDIO_PRINT("---> audio msDrvAudioDPGA_RecoverVolume: p:%d,  \r\n", SpkrMute);
			#endif
        }
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudioSourceSel()
/// @brief \b Function \b Description: Select audio source
/// @param <IN>        \b src: Line-in0, Line-in1, Line-in1, DAC0, DAC1, DAC2, DAC3, HDMI
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioSourceSel( AUDIO_SOURCE_SEL src )
{
    
    #if (AUDIO_STATE_DEBUG)
        MSAUDIO_PRINT("------- Set msDrvAudioSourceSel = %x ------- \r\n",src);
    #endif

        switch(src)
        {
            case AUDIO_SOURCE_HDMI:
                msWrite2ByteMask(REG_112C04, src<<12, BIT14|BIT13|BIT12);  //inmux switch
    		#if 0//ENABLE_I2S_PassThroughDAC
                msWrite2ByteMask( REG_112C04, 	BIT6|BIT4|0x08 ,	BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0 ); //[6] test clock enable [5:4] MCLK divider [3:0] BCLK divider
    		#endif
                break;

            case AUDIO_SOURCE_LINEIN0:
                msWrite2ByteMask(REG_112C04, src<<12, BIT14|BIT13|BIT12);  //inmux switch
                msWrite2ByteMask(REG_112CA0, BIT11, BIT11|BIT10);  //Disable HDMI/DP mute enable
    		#if 0//ENABLE_I2S_PassThroughDAC
                msWrite2ByteMask( REG_112C04, 	BIT6|BIT4|0x08 ,	BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0 ); //[6] test clock enable [5:4] MCLK divider [3:0] BCLK divider
    		#endif
                break;

            case AUDIO_SOURCE_DP:
            	  msWrite2ByteMask(REG_112C04, 0, BIT14|BIT13|BIT12);  	     //inmux switch    
    		#if 0//ENABLE_I2S_PassThroughDAC
                msWrite2ByteMask( REG_112C04, 	BIT6|BIT4|0x08 ,	BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0 ); //[6] test clock enable [5:4] MCLK divider [3:0] BCLK divider
    		#endif
                break;

            case AUDIO_SOURCE_NONE:
                MSAUDIO_PRINT("------- Set Audio Source NONE ------- \r\n");
                break;

            default:
                msWrite2ByteMask(REG_112C04, src<<12, BIT14|BIT13|BIT12);


                break;
        }
        audio_pad_init_flag         = 0;

        msDrvAudio_SetDPGAMuteSrc(E_DPGA_SOURCE_CHANGE_MUTE_ON);
        //MSAUDIO_PRINT("--<< Audio Source Mute >>---\r\n");

		
}

#if EnableDACPowerDown
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msDrvAudioPowerDown()
/// @brief \b Function \b Description: Audio DAC Power Down Control
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioPowerDown( void )
{
    msDrvAudioDPGA_SetVolRegMute();
    msDrvAudioStandbyDePop();
}
#endif


#if ENABLE_MSTV_UART_DEBUG
//------------------------------------------------------------------------------
/// @brief \b Function \b Name: msDrvAudioSineGen()
/// @brief \b Function \b Description: Audio internal sine gen
/// @param <IN>        \b bEnable, u8SinGenFreq(250Hz~12000Hz), u8SinGenGAIN(-90~0db)
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msDrvAudioSineGen(Bool bEnable, WORD u8SinGenFreq, BYTE u8SinGenGAIN) //high nibble for LCH,low nibble for RCH
{
    BYTE u8SinGenFreqSel;
    BYTE u8SinGenGAINSel = (~(u8SinGenGAIN/6))+1;
    BYTE u8BackupSourceSEL;

    if(u8SinGenFreq<=250)
    {
        u8SinGenFreqSel = PCM_250Hz;
    }
    else if(u8SinGenFreq<=500)
    {
        u8SinGenFreqSel = PCM_500HZ;
    }
    else if(u8SinGenFreq<=1000)
    {
        u8SinGenFreqSel = PCM_1KHZ;
    }
    else if(u8SinGenFreq<=1500)
    {
        u8SinGenFreqSel = PCM_1500HZ;
    }
    else if(u8SinGenFreq<=2000)
    {
        u8SinGenFreqSel = PCM_2KHZ;
    }
    else if(u8SinGenFreq<=3000)
    {
        u8SinGenFreqSel = PCM_3KHZ;
    }
    else if(u8SinGenFreq<=4000)
    {
        u8SinGenFreqSel = PCM_4KHZ;
    }
    else if(u8SinGenFreq<=6000)
    {
        u8SinGenFreqSel = PCM_6KHZ;
    }
    else if(u8SinGenFreq<=8000)
    {
        u8SinGenFreqSel = PCM_8KHZ;
    }
    else if(u8SinGenFreq<=12000)
    {
        u8SinGenFreqSel = PCM_12KHZ;
    }
    else
    {
        u8SinGenFreqSel = PCM_16KHZ;
    }
    u8BackupSourceSEL=(msRead2Byte(REG_112C04)>>12)&0x03;
    msWrite2ByteMask(REG_112C04, ((bEnable)?(AUDIO_SOURCE_DMA_RD<<12):(u8BackupSourceSEL<<12)), BIT14|BIT13|BIT12);  //inmux switch
    msWrite2ByteMask(REG_112C1C ,((bEnable)?(BIT15):(0)),BIT15|BIT14);      //[15] REG_DMA_SYNTH_ENABLE; [14] REG_DMA_SYNTH_CHANGE_FREQ

    msWrite2ByteMask(REG_112C14,(u8SinGenFreqSel<<4)|(u8SinGenFreqSel),BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0);
    msWrite2ByteMask(REG_112C16,(u8SinGenGAINSel<<4)|(u8SinGenGAINSel),BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0);
    msWrite2ByteMask(REG_112C14, ((bEnable)?(BIT15|BIT14|BIT13):(0))    ,BIT15|BIT14|BIT13);
    msDrvAudio_SetDPGAMuteSrc(E_DPGA_SOURCE_CHANGE_MUTE_OFF);
   
     
}
#endif



//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///           AUDIO API FUNCTION
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//******************************************************************************
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAPI_AudioInit()
/// @brief \b Function \b Description: Audio Init
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioInit( void )
{
    //MSAUDIO_PRINT("... Audio Initialize ...\r\n");
    msDrvAudioSetInit();
    msDrvAudioDACInit();
    msDrvAudioSourceSel(AUDIO_SOURCE_LINEIN0);
    msDrvAudio_SetEarphoneMute(TRUE);
    msDrvAudio_SetDPGAMuteSrc(E_DPGA_INIT_MUTE_ON);
}
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAPI_AudioPoweronDePop()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioPoweronDePop( void )
{
    #if EnableDACDePop
        msDrvAudioPoweronDePop(AudioDePopSequence);
    #endif
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAPI_AudioWakeUpDePop()
/// @brief \b Function \b Description:
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioWakeUpDePop( void )
{
    #if EnableDACDePop
        msDrvAudioWakeUpDePop(AudioWKUPDePopSequence);
    #endif
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAPI_AudioHandler()
/// @brief \b Function \b Description: Audio handler
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioHandler( void )
{
    if( (g_eBootStatus == eBOOT_STATUS_DCON) || (g_eBootStatus == eBOOT_STATUS_DPMSON))
    {
        msAPI_AudioWakeUpDePop();
    }
    else
    {
        msAPI_AudioPoweronDePop();
    }
}
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAPI_AudioSetMute()
/// @brief \b Function \b Description: Mute by Software from MCU to DMA Reader
/// @param <IN>        \b enable - 1: mute, 0: unmute
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioSetMute( AUDIO_MUTE_TYPE mute_type_sel, AUDIO_MUTE_SOURCE_SEL mute_src_type_sel )
{
    mute_type_sel=mute_type_sel;
    mute_src_type_sel=mute_src_type_sel;
}
//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAPI_AudioSetDpgaMute()
/// @brief \b Function \b Description: Mute from DPGA hardware block
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioSetDpgaMute( AUDIO_DPGA_SEL eDPGA, MS_BOOL enable )
{
    U16 temp_eDPGA;
    temp_eDPGA = eDPGA;

    if (enable)
    {
        msDrvAudioDPGA_SetVolRegMute();
        #if (AUDIO_STATE_DEBUG)
        MSAUDIO_PRINT("...............1 msDrvAudioDPGA_SetVolRegMute==>%x    \r\n",eDPGA);
        #endif
    }
    else
    {
        msDrvAudioDPGA_RecoverVolume();
        #if (AUDIO_STATE_DEBUG)
        MSAUDIO_PRINT("................2 msDrvAudioDPGA_RecoverVolume==>%x    \r\n",eDPGA);
        #endif
    }
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name:  msAPI_AudioSetEarphoneMute()
/// @brief \b Function \b Description: Earphone Mute.
/// @param <IN>        \b enable - 1: mute, 0: unmute
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioSetEarphoneMute( MS_BOOL enable )
{
    msDrvAudio_SetEarphoneMute(enable);
//    printData(" %%%%%%%%%%%%%%%%%%% msAPI_AudioSetEarphoneMute===%x",enable);
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name: msAPI_AudioSetVolume()
/// @brief \b Function \b Description: Set DPGA gain
/// @param <IN>        \b chsel: LR channel selection
///                      volume: volume control(0~ -113.875dB)
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioSetVolume( AUDIO_DPGA_SEL eDPGA, AUDIO_CH_SEL eChSelect, int iVolume_Integer, AUDIO_VOLUME_FRACTION_PART Volume_Fraction )
{
    msDrvAudioDPGASetVolume( eDPGA, eChSelect, iVolume_Integer, Volume_Fraction );
}

//------------------------------------------------------------------------------
/// @brief \b Function \b Name: msAPI_AudioSourceSel()
/// @brief \b Function \b Description: Select audio source
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioSourceSel( BYTE InputPort )
{
    AUDIO_SOURCE_SEL eAudioSrc = AUDIO_SOURCE_LINEIN0;
    INPUT_SRC_TYPE  eType_InputSrc;

    MSAUDIO_PRINT(" ---1. Audio InputPort = %x\n ",InputPort);
    //MSAUDIO_PRINT(" ---2. Audio Src = %x ---- \r\n",g_InputPort[InputPort].eIPType);

    if (INPUT_IS_VGA(InputPort))
    {
        eType_InputSrc = T_VGA;
        MSAUDIO_PRINT(" --- InputPort is VGA\n");
    }
    else if(INPUT_IS_DVI_S(InputPort))
    {
        eType_InputSrc = T_DVI_SINGLE;
        MSAUDIO_PRINT(" --- InputPort is DVI_1\n");
    }
    else if(INPUT_IS_DVI_D(InputPort))
    {
        eType_InputSrc = T_DVI_DUAL;
        MSAUDIO_PRINT(" --- InputPort is DVI_2\n");
    }
    else if((INPUT_IS_DVI_S(InputPort) | INPUT_IS_DVI_D(InputPort)))
    {
        eType_InputSrc = T_DVI;
        MSAUDIO_PRINT(" --- InputPort is DVI_3\n");
    }
    else if(INPUT_IS_VD(InputPort))
    {
        eType_InputSrc = T_VD;
        MSAUDIO_PRINT(" --- InputPort is VD\n");
    }
    else if(INPUT_IS_HDMI(InputPort))
    {
        eType_InputSrc = T_HDMI;
        MSAUDIO_PRINT(" --- InputPort is HDMI\n");
    }
    else if(INPUT_IS_DVI(InputPort) | INPUT_IS_HDMI(InputPort))
    {
        eType_InputSrc = T_MHL;
        MSAUDIO_PRINT(" --- InputPort is MHL\n");
    }
    else if(INPUT_IS_DISPLAYPORT(InputPort))
    {
        eType_InputSrc = T_DP;
        MSAUDIO_PRINT(" --- InputPort is DP\n");
    }
    else if(INPUT_IS_HBR2_0(InputPort))
    {
        eType_InputSrc = T_HBR2_0;
        MSAUDIO_PRINT(" --- InputPort is HBR\n");
    }
    else if(INPUT_IS_HBR2_1(InputPort))
    {
        eType_InputSrc = T_HBR2_1;
        MSAUDIO_PRINT(" --- InputPort is HBR\n");
    }
    else if(INPUT_IS_HBR2(InputPort))
    {
        eType_InputSrc = T_HBR2;
        MSAUDIO_PRINT(" --- InputPort is HBR\n");
    }
    else
    {
        eType_InputSrc = T_RESERVED;
        MSAUDIO_PRINT(" --- InputPort is RESERVED\n");
    }

    //MSAUDIO_PRINT("---> eType_InputSrc = %x\r\n",eType_InputSrc);
    switch(eType_InputSrc)
    {
        case T_VGA:
        case T_DVI_SINGLE:
        case T_DVI_DUAL:
        case T_DVI:
        case T_VD:
	#if defined(KTC_Project)
         eAudioSrc = AUDIO_SOURCE_NONE;
	#else
        eAudioSrc = AUDIO_SOURCE_LINEIN0;
        msAPI_combo_IPCreateAudioPath((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux);   //disable audio setting of combo part
        //MSAUDIO_PRINT(" --- Audio is Line-IN\n");
        #endif
            break;

        case T_HDMI:
        case T_MHL:
            eAudioSrc = AUDIO_SOURCE_HDMI;
            msAPI_combo_IPCreateAudioPath((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux);
            //MSAUDIO_PRINT(" --- Audio is HDMI\n");
            break;

        case T_DP:
        case T_HBR2_0:
        case T_HBR2_1:
        case T_HBR2:
            eAudioSrc = AUDIO_SOURCE_DP;
            msAPI_combo_IPCreateAudioPath((EN_COMBO_IP_SELECT)g_InputPort[InputPort].eIPMux);
            //MSAUDIO_PRINT(" --- Audio is DP\n");
            break;

        case T_RESERVED:
        default:
            eAudioSrc = AUDIO_SOURCE_NONE;
            //MSAUDIO_PRINT(" --- Audio is Reserved\n");
            break;
    }
    msDrvAudioSourceSel(eAudioSrc);
}
//------------------------------------------------------------------------------

#if EnableDACPowerDown
//------------------------------------------------------------------------------
/// @brief \b Function \b Name: msAPI_AudioPowerDown()
/// @brief \b Function \b Description: to power down Audio for saving consumption
/// @param <IN>        \b
/// @return <OUT>      \b
//------------------------------------------------------------------------------
void msAPI_AudioPowerDown(void )
{
    msDrvAudioPowerDown();
}
#endif
#endif  /* #define AUDIO_DELAY */

#endif

