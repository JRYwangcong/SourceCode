#include "types.h"
#include "reg52.h"
#include "board.h"
#include "debug.h"
//#include "common_debug.h"
#include "global.h"
#include "Ms_Reg.h"
#include "Ms_RWReg.h"
#ifdef MS_PM
#include "msPM.h"
#endif
#include "i2c.h"
#include "Debug.h"

#if IT6251

#define UNSTABLE_THRESHOLD 10

//#define DUMP_INPUTSTAT

// LVDS
// LVDS reg2C
#define B_LVDS_6Bpp   0
#define B_LVDS_8Bpp   1
#define B_LVDS_10Bpp  2
#define B_LVDS_12Bpp  3
#define B_JEIDA 0
#define B_VESA (1<<4)
#define B_ENABLE_DESSC (0<<6)
#define B_DISABLE_DESSC (1<<6)

// LVDS reg3C
#define B_LVDS_DESSC_ON 7
#define B_LVDS_DESSC_OFF 0

// LVDS reg35
#define B_DCBAL (1<<4)
// LVDS reg32
#define B_AUTO_DCBAL_DISABLE (1<<2)
#define B_AUTO_DCBAL_ENABLE  (0<<2)

// DPTX reg16
#define B_LBR    1
#define B_HBR    0

#define B_4_LANE (3<<1)
#define B_2_LANE (1<<1)
#define B_1_LANE (0<<1)

#define B_SSC_ENABLE   (1<<4)
#define B_SSC_DISABLE   (0<<4)
// DPTX reg60
#define B_HALF_PCLK     (1<<1)
// DPTX reg61
#define B_SINGLE_INPUT  (0<<2)
#define B_DUAL_INPUT    (1<<2)

// DPTX reg62[3:0]
#define B_DPTXIN_6Bpp   0
#define B_DPTXIN_8Bpp   1
#define B_DPTXIN_10Bpp  2
#define B_DPTXIN_12Bpp  3

#define AUXWAITTIME     10 // in ms
#define VIC 0 // non-Zero value for CEA setting, check the given input format.

// if LVDS is map1 , use default
// otherwise, define LVDS_MAP3
// the setting in config.h

#if PanelType == PanelAUOM215HW03
#define DUAL_MODE
#endif
// #define LVDS_MAP3
// #define ENABLE_LVDS_DESSC
#if(PanelType == PanelB156XTN02||PanelType == PanelB156XTN07||PanelType == PanelNT156WHM_N12/*||PanelType == PanelM185BGE_L23*/||PanelType == PanelAUOM215HW03\
||PanelType==PanelLGLP156WHB_DP||PanelType == PanelINNN173FGE_E23_DP||PanelType == PanelM185WHB_N10||||PanelType == PanelLM185TT3A)
#define COLOR_6BPP
#else
#define COLOR_8BPP
#endif
// #define COLOR_10BPP
// #define COLOR_12BPP

// #define ENABLE_DPTX_SSC
// #define FORCE_LBR

#ifndef LANE_COUNT
// option for lane count
///#define LANE_COUNT  B_4_LANE
#if(PanelType == PanelB156XTN02||PanelType == PanelB156XTN07||PanelType == PanelNT156WHM_N12||PanelType == PanelM185BGE_L23||PanelType == PanelLGLP156WHB_DP\
||PanelType == PanelINNN173FGE_E23_DP||PanelType == PanelM185WHB_N10||||PanelType == PanelLM185TT3A)
#define LANE_COUNT  B_1_LANE
#else
 #define LANE_COUNT  B_2_LANE
 #endif
#endif


#ifdef FORCE_LBR
    #define TRAINING_BITRATE     B_LBR
#else
    #define TRAINING_BITRATE     B_HBR
#endif

// option for SSC

#ifdef ENABLE_DPTX_SSC
    #define DPTX_SSC_SETTING         B_SSC_ENABLE
#else
    #define DPTX_SSC_SETTING         B_SSC_DISABLE
#endif

#ifndef UNSTABLE_THRESHOLD
    #define UNSTABLE_THRESHOLD 10
#endif

#ifdef LVDS_MAP3
    #define LVDS_MAP    B_VESA
#else
    #define LVDS_MAP    B_JEIDA
#endif

#ifdef ENABLE_LVDS_DESSC
    #define LVDS_DESSC         B_LVDS_DESSC_ON
    #define LVDS_DESSC_SETTING B_ENABLE_DESSC
#else
    #define LVDS_DESSC         B_LVDS_DESSC_OFF
    #define LVDS_DESSC_SETTING B_DISABLE_DESSC
#endif


// options for colordepth
#ifdef COLOR_6BPP
    #define LVDS_BPP B_LVDS_6Bpp
    #define DPTX_BPP B_DPTXIN_6Bpp
#elif defined(COLOR_10BPP)
    #define LVDS_BPP B_LVDS_10Bpp
    #define DPTX_BPP B_DPTXIN_10Bpp
#elif defined(COLOR_12BPP)
    #define LVDS_BPP B_LVDS_12Bpp
    #define DPTX_BPP B_DPTXIN_12Bpp
#else // COLOR_8BPP
    #define LVDS_BPP B_LVDS_8Bpp
    #define DPTX_BPP B_DPTXIN_8Bpp
#endif

#define INT_MASK 7

typedef struct {
	BYTE addr ;
	BYTE offset ;
	BYTE andmask ;
	BYTE ormask ;
} I2C_REG_VALUE ;

code I2C_REG_VALUE IT6251_InitTable[] =
{
    {DP_I2C_ADDR,0x05,0xFF,0xff},
    {DP_I2C_ADDR,0x05,0xFF,0x00},
    {DP_I2C_ADDR,0xfd,0xFF,LVDS_I2C_ADDR},
    {DP_I2C_ADDR,0xfe,0xFF,0x01},

    // LVDS Configuration
    {LVDS_I2C_ADDR,0x05,0xFF,0xff},
    {LVDS_I2C_ADDR,0x05,0xFF,0x00},
    {LVDS_I2C_ADDR,0x3b,0xFF,0x42},
    {LVDS_I2C_ADDR,0x3b,0xFF,0x43},
    {LVDS_I2C_ADDR,0x3c,0xFF,0x08|LVDS_DESSC},
    {LVDS_I2C_ADDR,0x0b,0xFF,0x88},

    // LVDS reg2C
    // #define B_LVDS_6Bpp   0
    // #define B_LVDS_8Bpp   1
    // #define B_LVDS_10Bpp  2
    // #define B_LVDS_12Bpp  3
    // #define B_JEIDA 0
    // #define B_VESA (1<<4)
    // #define B_ENABLE_DESSC (0<<6)
    // #define B_DISABLE_DESSC (1<<6)

    {LVDS_I2C_ADDR,0x2c,0xFF,LVDS_DESSC_SETTING|LVDS_MAP|LVDS_BPP},

    {LVDS_I2C_ADDR,0x32,0xFF,0x04},
    {LVDS_I2C_ADDR,0x35,0xFF,0xe0},
    {LVDS_I2C_ADDR,0x05,0xFF,0x02},
    {LVDS_I2C_ADDR,0x05,0xFF,0x00},

    {DP_I2C_ADDR,0x09,0xFF,INT_MASK}, // Enable HPD_IRQ, HPD_CHG, VIDSTABLE
    {DP_I2C_ADDR,0x0A,0xFF,0x00},
    {DP_I2C_ADDR,0x0B,0xFF,0x00},
    {DP_I2C_ADDR,0x06,0xFF,0xFF}, // Clear all interrupt
    {DP_I2C_ADDR,0x07,0xFF,0xFF}, // Clear all interrupt
    {DP_I2C_ADDR,0x08,0xFF,0xFF}, // Clear all interrupt

    {DP_I2C_ADDR,0x16,0x17,DPTX_SSC_SETTING|LANE_COUNT|TRAINING_BITRATE},
    {DP_I2C_ADDR,0x23,0xFF,0x40},
    {DP_I2C_ADDR,0x5c,0xFF,0xf3},
    {DP_I2C_ADDR,0x5f,0xFF,0x06},

#ifdef DUAL_MODE
    {DP_I2C_ADDR,0x60,0xFF,B_HALF_PCLK},
    {DP_I2C_ADDR,0x61,0xFF,B_DUAL_INPUT},
#else
    {DP_I2C_ADDR,0x60,0xFF,0x00},
    {DP_I2C_ADDR,0x61,0xFF,0x00},
#endif

    //{DP_I2C_ADDR,0x62,0xFF,0X40|DPTX_BPP},


    {DP_I2C_ADDR,0x62,0xFF,DPTX_BPP},

    {DP_I2C_ADDR,0xa0,0xFF,0x00},
    {DP_I2C_ADDR,0xc9,0xFF,0xf5},
    {DP_I2C_ADDR,0xca,0xFF,0x4d},
    {DP_I2C_ADDR,0xcb,0xFF,0x37},
    // {DP_I2C_ADDR,0xce,0xFF,0x60},
    {DP_I2C_ADDR,0xd3,0xFF,0x03},
    {DP_I2C_ADDR,0xd4,0xFF,0x45},
    {DP_I2C_ADDR,0xe7,0xFF,0xa0},
    {DP_I2C_ADDR,0xe8,0xFF,0x33},
    {DP_I2C_ADDR,0xec,0xFF,VIC},
    {DP_I2C_ADDR,0x23,0xFF,0x42},
    {DP_I2C_ADDR,0x24,0xFF,0x00},
    {DP_I2C_ADDR,0x25,0xFF,0x00},
    {DP_I2C_ADDR,0x26,0xFF,0x00},
    {DP_I2C_ADDR,0x2b,0xFF,0x00},
    {DP_I2C_ADDR,0x23,0xFF,0x40},
    {DP_I2C_ADDR,0x17,0xFF,0x04},
    {DP_I2C_ADDR,0x17,0xFF,0x01},

#if PanelType==PanelLGLP156WHB_DP//USE ITE  DE  MODE
    {DP_I2C_ADDR,0x79,0xFF,0x20},
    {DP_I2C_ADDR,0x7A,0xFF,0x49},
    {DP_I2C_ADDR,0x7B,0xFF,0x06},
    {DP_I2C_ADDR,0x7C,0xFF,0xF3},
    {DP_I2C_ADDR,0x7D,0xFF,0x00},
    {DP_I2C_ADDR,0x7E,0xFF,0x49},
    {DP_I2C_ADDR,0x7F,0xFF,0x06},
    {DP_I2C_ADDR,0x80,0xFF,0x2F},
    {DP_I2C_ADDR,0x81,0xFF,0x00},
    {DP_I2C_ADDR,0x82,0xFF,0x4F},
    {DP_I2C_ADDR,0x83,0xFF,0x00},
    {DP_I2C_ADDR,0x84,0xFF,0x15},
    {DP_I2C_ADDR,0x85,0xFF,0x03},
    {DP_I2C_ADDR,0x86,0xFF,0x15},
    {DP_I2C_ADDR,0x87,0xFF,0x00},
    {DP_I2C_ADDR,0x88,0xFF,0x15},
    {DP_I2C_ADDR,0x89,0xFF,0x03},
    {DP_I2C_ADDR,0x8A,0xFF,0xFF},
    {DP_I2C_ADDR,0x8B,0xFF,0x0F},
    {DP_I2C_ADDR,0x8C,0xFF,0xFF},
    {DP_I2C_ADDR,0x8D,0xFF,0x0F},
    {DP_I2C_ADDR,0x8E,0xFF,0x03},
    {DP_I2C_ADDR,0x8F,0xFF,0x00},
    {DP_I2C_ADDR,0x90,0xFF,0x08},
    {DP_I2C_ADDR,0x91,0xFF,0xFF},
    {DP_I2C_ADDR,0x92,0xFF,0xFF},
    {DP_I2C_ADDR,0x93,0xFF,0xFF},
    {DP_I2C_ADDR,0x94,0xFF,0x00},
    {DP_I2C_ADDR,0x95,0xFF,0xFF},
    {DP_I2C_ADDR,0x96,0xFF,0x00},
    {DP_I2C_ADDR,0x97,0xFF,0x00},
#endif

};

#define Count_IT6251_InitTable (sizeof(IT6251_InitTable)/sizeof(I2C_REG_VALUE))
BYTE xdata PrevHPD = FALSE ;
BYTE xdata HPD ;
BYTE xdata VidStable = FALSE ;
BYTE xdata VidUnStableCount = 0 ;


void ForceDelay1ms(WORD msNums);
void Delay1ms(WORD msNums);


void InitIT6251(void)
{
    int i,j;
    BYTE ucValue ;
    BYTE linkstat ;
//printData("InitIT6251",1);
    for( i = 0 ; i < Count_IT6251_InitTable ; i++ )
    {
        if(IT6251_InitTable[i].andmask != 0xFF )
        {
            ucValue=ReadI2C_Byte(IT6251_InitTable[i].addr,
                        IT6251_InitTable[i].offset );

            ucValue &= ~(IT6251_InitTable[i].andmask) ;
            ucValue |= IT6251_InitTable[i].ormask ;
            WriteI2C_Byte(IT6251_InitTable[i].addr,
                        IT6251_InitTable[i].offset,
                        ucValue );
        }
        else
        {
            WriteI2C_Byte(IT6251_InitTable[i].addr,
                        IT6251_InitTable[i].offset,
                        IT6251_InitTable[i].ormask);
        }
    }
	// 2011/08/01 modified by Alin
    ucValue=ReadI2C_Byte(DP_I2C_ADDR, 0x0D);
    PrevHPD=(ucValue&0x02)?TRUE:FALSE ;
// checking timeout, in 500ms

  for(j=0 ; j<20;j++)

    {
       //CTimerDelayXms(20);
	ForceDelay1ms(20);
        for(i=0 ; i <200 ; i ++ )
        {
 	     //CTimerDelayXms(5);
	     ForceDelay1ms(5);
            linkstat = ReadI2C_Byte(DP_I2C_ADDR, 0x0E);
            if((linkstat & 0x1F)==0x10)
            {
              //  normal operation
                // check if the training bitrate same to bitrate
                if(linkstat & 0x80)
                {
                    ucValue = B_HBR ;
                }
                else
                {
                    ucValue = B_LBR ;
                }

                if( ucValue == TRAINING_BITRATE )
                {
                   return ;
                }
                else
                {
                   break ;
                }
            }
        }

        // training result fail, retrain
       WriteI2C_Byte(DP_I2C_ADDR, 0x05, 0x00);
       WriteI2C_Byte(DP_I2C_ADDR, 0x17, 0x04);
        WriteI2C_Byte(DP_I2C_ADDR, 0x17, 0x01);

	
    }


//WriteI2C_Byte(DP_I2C_ADDR, 0x17, 0x01);//
}

#if IT6251

void CheckIT6251(void)
{
    BYTE reg06, ucStat ;

    reg06 = ReadI2C_Byte(DP_I2C_ADDR, 0x06) ;

//printData("44444444 reg06==%d",reg06);

    if( reg06 == 0 ) return ;
    WriteI2C_Byte(DP_I2C_ADDR, 0x06, reg06) ; // clear interrupt, at first

    if( reg06 & 7 )
    {
        ucStat = ReadI2C_Byte(DP_I2C_ADDR, 0x0D) ;

        if( (reg06 & 3 )&&(ucStat & 2 )) // HPD change or HPDIRQ and HPD , reinit.
        {
            InitIT6251() ;
            return ;
        }

        if( reg06 & 4 )
        {
            if( VidUnStableCount < UNSTABLE_THRESHOLD )
            {
                VidUnStableCount++ ;
            }
            else
            {
                InitIT6251() ;
                VidUnStableCount = 0 ;
                return ;
            }
        }
    }
}

// polling
void IT6251_DeviceLoopProcedure(void)
{
    BYTE ucStat;

    CheckIT6251();

    ucStat = ReadI2C_Byte(DP_I2C_ADDR, 0x0D) ;
    VidStable = (ucStat & 4)?TRUE:FALSE ;
    HPD = (ucStat & 2)?TRUE:FALSE ;

#ifndef NO_LOOP_CHECKING_HPD
    if( HPD != PrevHPD )
    {
        PrevHPD = HPD ;
        if( HPD )
        {
            InitIT6251() ;
        }
    }
#endif

#ifndef NO_LOOP_CHECKING_VIDSTABLE
    if( VidStable )
    {
        if( VidUnStableCount >= UNSTABLE_THRESHOLD )
        {
            InitIT6251() ;
            VidUnStableCount = 0 ;
        }

        if( VidUnStableCount > 0 )
        {
            VidUnStableCount -- ;
        }
    }
    else
    {
        if( VidUnStableCount < UNSTABLE_THRESHOLD )
        {
            VidUnStableCount++ ;
        }
    }

#endif

}

#if 0
void IT6251_PowerOn()
{
/*
    WriteI2C_Byte(DP_I2C_ADDR, 0x05, 0xff) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0x05, 0x3b) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0xfd, 0xbc) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0xfe, 0x01) ;
    WriteI2C_Byte(LVDS_I2C_ADDR, 0x05, 0x00) ;
    WriteI2C_Byte(LVDS_I2C_ADDR, 0x0b, 0x88) ;
    WriteI2C_Byte(LVDS_I2C_ADDR, 0x3b, 0x43) ;
    WriteI2C_Byte(LVDS_I2C_ADDR, 0x3c, 0x08) ;
    WriteI2C_Byte(LVDS_I2C_ADDR, 0x52, 0x13) ;
	
    WriteI2C_Byte(DP_I2C_ADDR, 0x0c, 0x08) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0x58, 0x07) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0x59, 0x29) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0x5a, 0x03) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0x5b, 0x64) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0x5c, 0x03) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0x5e, 0x16) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0x10, 0x00) ;
*/	
    InitIT6251() ;
}
#endif

void IT6251_PowerDown()
{
/*
    printData("LVDS 0x05  is  ==%x",(ReadI2C_Byte(LVDS_I2C_ADDR, 0x05)));
    printData("LVDS 0x0B  is  ==%x",(ReadI2C_Byte(LVDS_I2C_ADDR, 0x0B)));
    printData("LVDS 0x3b  is  ==%x",(ReadI2C_Byte(LVDS_I2C_ADDR, 0x3b)));
    printData("LVDS 0x3c  is  ==%x",(ReadI2C_Byte(LVDS_I2C_ADDR, 0x3c)));
    printData("LVDS 0x52  is  ==%x",(ReadI2C_Byte(LVDS_I2C_ADDR, 0x52)));


    printData("DP 0x05  is  ==%x",(ReadI2C_Byte(DP_I2C_ADDR, 0x05)));
    printData("DP 0xfd  is  ==%x",(ReadI2C_Byte(DP_I2C_ADDR, 0xfd)));
    printData("DP 0xfe  is  ==%x",(ReadI2C_Byte(DP_I2C_ADDR, 0xfe)));

    printData("DP 0x0c  is  ==%x",(ReadI2C_Byte(DP_I2C_ADDR, 0x0c)));
    printData("DP 0x58  is  ==%x",(ReadI2C_Byte(DP_I2C_ADDR, 0x58)));
    printData("DP 0x59  is  ==%x",(ReadI2C_Byte(DP_I2C_ADDR, 0x59)));
    printData("DP 0x5A  is  ==%x",(ReadI2C_Byte(DP_I2C_ADDR, 0x5A)));
    printData("DP 0x5B  is  ==%x",(ReadI2C_Byte(DP_I2C_ADDR, 0x5B)));
    printData("DP 0x5C  is  ==%x",(ReadI2C_Byte(DP_I2C_ADDR, 0x5C)));
    printData("DP 0x5E  is  ==%x",(ReadI2C_Byte(DP_I2C_ADDR, 0x5E)));
    printData("DP 0x10  is  ==%x",(ReadI2C_Byte(DP_I2C_ADDR, 0x10)));
*/
    WriteI2C_Byte(DP_I2C_ADDR, 0x05, 0xff) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0x05, 0x3b) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0xfd, 0xbc) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0xfe, 0x01) ;
    WriteI2C_Byte(LVDS_I2C_ADDR, 0x05, 0x02) ;
    WriteI2C_Byte(LVDS_I2C_ADDR, 0x0b, 0x08) ;
    WriteI2C_Byte(LVDS_I2C_ADDR, 0x3b, 0x40) ;
    WriteI2C_Byte(LVDS_I2C_ADDR, 0x3c, 0x08) ;
    WriteI2C_Byte(LVDS_I2C_ADDR, 0x52, 0x00) ;	
/*
    WriteI2C_Byte(DP_I2C_ADDR, 0x0c, 0x00) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0x58, 0x10) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0x59, 0x20) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0x5a, 0x0c) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0x5b, 0x42) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0x5c, 0x02) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0x5e, 0x14) ;
    WriteI2C_Byte(DP_I2C_ADDR, 0x10, 0x7c) ;
*/
}



#ifdef DUMP_INPUTSTAT
void dptx_show_vid_info( void )
{


    WORD HSyncPol, VSyncPol, InterLaced;
    WORD HTotal, HDES, HDEW, HFPH, HSYNCW;
    WORD VTotal, VDES, VDEW, VFPH, VSYNCW;
    WORD rddata;
	//DWORD PCLK, sum;
    BYTE sysstat,linkstat ;


    sysstat = ReadI2C_Byte(DP_I2C_ADDR,0x0D);
    linkstat = ReadI2C_Byte(DP_I2C_ADDR,0x0E);
/*
    if(linkstat & 0x1F != 0x10 )
    {
        DPTX_DEBUG_PRINTF(("Link Training has something wrong, reg0E = %02X\n",(int)linkstat)) ;
    }
    else
    {
        DPTX_DEBUG_PRINTF(("reg0E = %02X, Link Rate = %s",(int)linkstat,(ReadI2C_Byte(DP_I2C_ADDR,0x0E)&0x80)?"HBR":"LBR")) ;
    }

    DPTX_DEBUG_PRINTF(("reg0D = %02X, %s%s%s\n",(int)sysstat,(sysstat&1)?"Interrupt! ":"",(sysstat&2)?"HPD ":"Unplug " ,(sysstat&4)?"Video Stable":"Video Unstable" )) ;
*/
    if( (sysstat & 0x04 ==0) )
    {
        return ;
    }

    rddata = ReadI2C_Byte(DP_I2C_ADDR,0xA0);
    HSyncPol = rddata&0x01;
    VSyncPol = (rddata&0x04)>>2;
    InterLaced = (rddata&0x10)>>4;

    rddata = ReadI2C_Byte(DP_I2C_ADDR,0xA1);
    HTotal = ((ReadI2C_Byte(DP_I2C_ADDR,0xA2)&0x1F)<<8) + rddata;

    rddata = ReadI2C_Byte(DP_I2C_ADDR,0xA3);
    HDES = ((ReadI2C_Byte(DP_I2C_ADDR,0xA4)&0x03)<<8) + rddata;

    rddata = ReadI2C_Byte(DP_I2C_ADDR,0xA5);
    HDEW = ((ReadI2C_Byte(DP_I2C_ADDR,0xA6)&0x1F)<<8) + rddata;

    rddata = ReadI2C_Byte(DP_I2C_ADDR,0xA7);
    HFPH = ((ReadI2C_Byte(DP_I2C_ADDR,0xA8)&0x03)<<8) + rddata;

    rddata = ReadI2C_Byte(DP_I2C_ADDR,0xA9);
    HSYNCW = ((ReadI2C_Byte(DP_I2C_ADDR,0xAA)&0x03)<<8) + rddata;

    rddata = ReadI2C_Byte(DP_I2C_ADDR,0xAB);
    VTotal = ((ReadI2C_Byte(DP_I2C_ADDR,0xAC)&0x0F)<<8) + rddata;

    rddata = ReadI2C_Byte(DP_I2C_ADDR,0xAD);
    VDES = ((ReadI2C_Byte(DP_I2C_ADDR,0xAE)&0x01)<<8) + rddata;

    rddata = ReadI2C_Byte(DP_I2C_ADDR,0xAF);
    VDEW = ((ReadI2C_Byte(DP_I2C_ADDR,0xB0)&0x0F)<<8) + rddata;

    rddata = ReadI2C_Byte(DP_I2C_ADDR,0xB1);
    VFPH = ((ReadI2C_Byte(DP_I2C_ADDR,0xB2)&0x01)<<8) + rddata;

    rddata = ReadI2C_Byte(DP_I2C_ADDR,0xB3);
    VSYNCW = ((ReadI2C_Byte(DP_I2C_ADDR,0xB4)&0x01)<<8) + rddata;

	printData("HDEW==%d", HDEW);
	printData("VDEW==%d", VDEW);

	printData("HTotal==%d", HTotal);
	printData("VTotal==%d", VTotal);

	printData("HFPH==%d", HFPH);
	printData("HSYNCW==%d", HSYNCW);

	printData("VFPH==%d", VFPH);
	printData("VSYNCW==%d", VSYNCW);

	printData("HDES==%d", HDES);
	printData("VDES==%d", VDES);

    //DPTX_DEBUG_PRINTF(("%4dx%4d,",HDEW,VDEW)) ;
    //DPTX_DEBUG_PRINTF(("(%4dx%4d),",HTotal,VTotal)) ;
    //DPTX_DEBUG_PRINTF(("(%d,%d,%d),", HFPH, HSYNCW  , HTotal - HDEW - HFPH - HSYNCW));
    //DPTX_DEBUG_PRINTF(("(%d,%d,%d),",VFPH,VSYNCW,VTotal - VDEW - VFPH - VSYNCW)) ;
    //DPTX_DEBUG_PRINTF(("%s",(InterLaced&0x01)?"INTERLACED":"PROGRESS")) ;
    //DPTX_DEBUG_PRINTF(("{H%s,V%s} ",(HSyncPol&0x01)?"-":"+",(VSyncPol&0x01)?"-":"+")) ;
/*
    rddata = ReadI2C_Byte(DP_I2C_ADDR,0x12) ;
    rddata |= 0x80 ;
    WriteI2C_Byte(DP_I2C_ADDR,0x12, (BYTE)rddata) ;
    rddata &= 0x7f ;
    delay1ms(1) ;
    WriteI2C_Byte(DP_I2C_ADDR,0x12, (BYTE)rddata) ;

    sum = ReadI2C_Byte(DP_I2C_ADDR,0x13);
    sum = ((ReadI2C_Byte(DP_I2C_ADDR,0x14)&0x0F)<<8)+rddata;
    PCLK = (13500L*2048L)/sum;
    DPTX_DEBUG_PRINTF(("PCLK = %ldHz, xCnt = %d\n",PCLK,sum)) ;
*/

}
#endif// DUMP_INPUTSTAT

#endif// DUMP_INPUTSTAT

#else


BYTE code tITENonUse[]=
{
  0xFF,0xFF
};
void ITENonUse(void)
{
  BYTE i;

  i=tITENonUse[0];
}


#endif// DUMP_INPUTSTAT

