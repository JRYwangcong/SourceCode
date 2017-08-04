#include "board.h"
typedef enum msbus_direction
{
    MS_WRITE, MS_READ
} msBus_Direction;
// Register Bank Select
#define REGBK	0x00
#define AINC	BIT3
#define BUST	BIT2
#define REGBANK_MASK	0x3
#define REGBANKSCALER	0x0	//Scaler Bank
#define REGBANKADC		0x1	// ADC Bank
#define REGBANKTCON	0x2	// TCON Bank

#define REGBANK0		0x00
#define REGBANK1		0x01
#define REGBANK2		0x02
#define REGBANK3		0x03
#define REGBANK4		0x04
#define REGBANK5		0x05
#define REGBANK6		0x06
#define REGBANK7		0x07
#define REGBANK9		0x09
#define REGBANKA		0x0A
#define REGBANKE		0x0E

// Bank = 01
// Double buffer control
#define DBFC	0x01		// enable dobule buffer while vertical blanking
#define DBC_B	BIT0
#define DBL_0	0
#define DBL_1	2
#define DBL_2	4
#define DBL_3	6
//PLL Divider control
#define PLLDIVM		0x02	// PLL divider (HTotal)
#define PLLDIVL		0x03
// input Gain
#define REDGAIN		0x04
#define GRNGAIN		0x05
#define BLUGAIN		0x06
// input Offset
#define REDOFST		0x07
#define GRNOFST		0x08
#define BLUOFST		0x09
// Clamp Timing
#define CLPLACE		0x0A	// clamping placement
#define CLDUR		0x0B	// clamping duration
// General Control
#define GCTRL		0x0C
#define DRBS	BIT0	// DVI R/B swap
#define CSTP	BIT1	// Coast polarity  1(d)/0 = H/L
#define WDIS	BIT2	// disable pll watchdog timer
#define CCDIS	BIT3	// disable clamp during active coast
#define CLPE	BIT4	// Clamping edge 0/1= leading/trailing
#define HSLE	BIT5	// hsync lock edge 0/1=leading/trailing
#define ECLK	BIT6	// internal/external clcok
#define HSP_B	BIT7	// Hsync Polarity 1/0 = H/L
// PLL coefficient
#define BWCOEF		0x0D
#define FCOEF		0x0E
#define DCOEF		0x0F
//Clock Phase control
#define CLKCTRL1	0x10
#define CLKCTRL2	0x11
// VCO control
#define VCOCTRL		0x12
// Power management
#define RT_CTL		0x13
// SOG/HSYNC programming levle
#define SOG_LVL		0x14
#define OFIR_B	BIT5
#define BMID_B	BIT6
#define RMID_B	BIT7
#define HS_LVL		0x15
#define RT_STATUS1	0x17
#define RT_STATUS2	0x18
#define PHR_STATUS1	0x19
#define PHR_STATUS2	0x1A
#define DVI_PHR		0x1B
#define DVI_PHG		0x1C
#define DVI_PHB		0x1D
#define DVI_ERST	0x1E
#define DVI_ERTH	0x1F
#define TESTEN		0x20
#define RED_CHANNEL	0x20
#define GREEN_CHANNEL	0x10
#define BLUE_CHANNEL	0x00
#define RDST_B	BIT2
#define ERRD_B	BIT3
//bit4..5 ERROR status channel select (2: Red, 1:Green, 0:Blue)
#define TSTEN_B	BIT7
#define TESTA0		0x21
#define TESTA1		0x22
#define TESTA2		0x23
#define TESTA3		0x24
#define TESTA4		0x25
#define TESTA5		0x26
#define DVIDET_B	BIT0 // DVI Clock Detection
#define DMUX_B		BIT1 // DVI demultiplexer
#define PHD_B		BIT2 // ADCPLL Phase Digitizer
#define DMIBEX_B	BIT3 // output current biase
#define DPLBG_B		BIT4 // Pll bandgap
#define AMUX_B		BIT5 // Analog Mux Override
#define OVRD_B		BIT7 // Power Down Overrides
#define TESTA6		0x27
#if 0
    #define VREF_B		BIT0 // Power down Hsync Voltatge
    #define AREF_B		BIT2 // power down ADC Voltage reference
    #define DPL_B		BIT4 // power down PLL
    #define ADCB_B		BIT5 // Power down ADC Red Channel
    #define DDCG_B		BIT6 // Power down ADC Red Channel
    #define ADCR_B		BIT7 // Power down ADC Red Channel
#endif 
#define TESTD0		0x28
#define TESTD1		0x29
#define TESTD2		0x2A
#define TESTD3		0x2B
#define TESTD4		0x2C
#define TESTMOD		0x2D
#define TST_REG		0x2E
#define OPAT_B	BIT6 // Output picture at test mode
#define EOVB_B	BIT7 // Enable override bonding option
//bit0..5 override bonding option
#define PLLCTRLV	0x30
// Bank = 00
// Graphic Port
#define ISELECT		0x02
#define IHSU_B	BIT2
#define CSC_B	BIT3
#define COMP_B	BIT4
#define NIS_B	            BIT7
#define STYPE_Mask	0x60
#define ISEL_Mask	 0xFC
#define ISEL_Analog1	0x00
#define ISEL_Analog2	0x01
#define ISEL_DVI	0x02
#define ISEL_Video	0x03
#define IPCTRL2		0x04
#define HWRP_B	BIT0 // input horizontal wrap
#define VWRP_B	BIT1 // input vertical wrap
#define ESLS_B	BIT2 // early sample line select
#define VSE_B	BIT3 // input vsync reference edge
#define HSE_B	BIT4 // input hsync reference edge
#define IVSD_B	BIT5 // vsync delay select
#define DEON_B	BIT6 // DE only
#define DHSR_B	BIT7 // Digital input horizontal range
// Input image sample range
#define SPRVST_L	           0x05	// V-start
#define SPRVST_H	           0x06
#define SPRHST_L	           0x07	// H-start
#define SPRHST_H	           0x08
#define SPRVDC_L	            0x09	// V-
#define SPRVDC_H	0x0A
#define SPRHDC_L     	0x0B	// H-D
#define SPRHDC_H	0x0C
#define LVL		            0x0F // Lock Y line
//Display Timing
#define DEVST_L		0x10	// V-start
#define DEVST_H		0x11
#define DEHST_L		0x12	// h start
#define DEHST_H		0x13
#define DEVEND_L	0x14	// v end
#define DEVEND_H	0x15
#define DEHEND_L	0x16	// h end
#define DEHEND_H	0x17
// scale image window size
#define SIHST_L		0x18
#define SIHST_H		0x19
#define SIVEND_L	0x1A
#define SIVEND_H	0x1B
#define SIHEND_L	0x1C
#define SIHEND_H	0x1D
//output sync timing
#define VDTOT_L		0x1E
#define VDTOT_H		0x1F
#define VSST_L		0x20
#define VSST_H		0x21
#define VSEND_L		0x22
#define VSEND_H		0x23
#define HDTOT_L		0x24
#define HDTOT_H		0x25
#define HSEND		0x26
// output sync control
#define OSCCTRL1	0x27
#define CTRL_B	BIT0
#define AHRT_B	BIT1
#define MOD2_B	BIT2
#define EHTT_B	BIT3
#define VSGP_B	BIT4
#define HSRM_B	BIT5
#define LCM_B	BIT6
#define AVOS_B	BIT7
#define OSCCTRL2	0x28
#define CRM_B	BIT0
#define SLE_B	BIT1
#define ATEN2_B	BIT5
//brightness control
#define BRC		0x2A
#define BRC_B	BIT0 // enable brightness function
#define BCR		0x2B
#define BCG		0x2C
#define BCB		0x2D
// contrast control
#define CNTR		0x2E
#define CNTR_B BIT0 // enable contrast function
#define CNTT_B	BIT1 // contrast type select, 0/1 set 0 or 128 as center
#define CCR		0x2F
#define CCG		0x30
#define CCB		0x31
// background color
#define FWC		0x32
#define FWC_B	BIT0 // border color on/off
#define FCR		0x33
#define FCG		0x34
#define FCB		0x35
// Dither control
#define DITHCTRL	0x36
#define DITH_B	BIT0 // enable dither function
#define OBN_B	BIT1 // Output Bit number 0/1:8/6
#define TROT_B	BIT2 // temporal coefficient rotate
#define SROT_B	BIT3 // Spatial Coefficient Rotate
#define DITHG_BM	0x30
#define DITHG_GM	0xC0
#define DITHCOEF	0x37
#define TRFN		0x38
// gamma control
#define GAMMAC		0x40
#define GCFE_B	BIT0 // enable gamma function
#define GTIO_B	BIT1 // enable gamme I/O access
#define GammaRed	 0
#define GammaGreen	0x4
#define GammaBlue	0x8
#define GammaAll	0xC
#define GAMMAP		0x41
//output control
#define OCTRL1		0x42
#define ERBX_B	BIT0
#define ORBX_B	BIT1
#define EMLX_B	BIT2
#define OMLX_B	BIT3
#define LTIM_B	BIT4
#define MLXT_B	BIT5
#define LCS_B	BIT6
#define LCPS_B	BIT7
#define OCTRL2		0x43
#define DPO_B	BIT0 // Dual output
#define DPX_B	BIT1 // switch output A, B
#define STO_B	BIT2 // stag
#define REV_B	BIT3 // revser luminosity
#define BLKS_B	BIT4 // black background
#define WHTS_B	BIT5 // white
#define DOT_B	BIT6 // differential output
#define TCOP_B	BIT7 // tcon control
#define OCTRL3		0x44
#define ENRT_B	BIT0 // enr TG
#define ENRL_B	BIT1 // enr TLRG
#define END_B	BIT2 // end TRG
#define ENU_B	BIT3 // enu TRG
#define PMOD	BIT7 // Power down MOD
#define TTL	0x00 // TTL Output
#define LVDS	0x01 // LVDS Output
#define RSDS_1	0x07 // RSDS Output with vide-side GPO
#define RSDS_2	0x15 // RSDS Output with LVDS-side GPO
#define RSDS	0x1F // RSDS Output with all GPO
//osd alpha blending control
#define BLENDC		0x4B
#define BLENDL		0x4C
#define PWMDIV0A9     0x4D
// scaling ratio
#define SRH_L		0x50
#define SRH_M		0x51
#define SRH_H		0x52
#define SENH_B	BIT7 // Enable Horizontal scaling
#define SRV_L		0x53
#define SRV_M		0x54
#define SRV_H		0x55
#define SENV_B	BIT7 // Enable Vertical scaling
// scaling filter control
#define SFH		0x56
#define SFV		0x57
#define HDSUSG		0x58
#define HDSUSL		0x59
#define VDSUSG		0x5A
#define VDSUSL		0x5B
#define PFEN               0x5C
#define PFCOEF           0x5D
// filter coefficient

#define PG_SWCH		0x63

#define FTAPEN		0x65
#define FTAPC1		0x66
#define FTAPC2		0x67
#define FTAPC3		0x68
#define FTAPC4		0x69
#define SRGBEN		0x65
#define SRGB12		0x66
#define SRGB13		0x67
#define SRGB21		0x68
#define SRGB23		0x69
#define SRGB31		0x6A
#define SRGB32		0x6B
// interlaced mode line shift
#define INTMDS		0x6F //
#define ODDF_B	BIT3 // Shift Odd field
#define ILIM_B	BIT4 // insert line
// auto gain function adjust
#define GPIO_INT         0x73
#define PWMDIV_HIGH_EN BIT0
#define ATGCTRL		0x78
#define ATGR_B BIT1
#define MAXB_B	BIT5
#define MAXG_B	BIT6
#define MAXR_B	BIT7
#define ATGST		0x79
#define MINB_B	BIT0
#define MING_B	BIT1
#define MINR_B	BIT2
//Auto position function and jitter function
#define ATOCTRL		0x7B
#define ATOR_B	BIT1
#define AOVDV		0x7C
// auto position function result
#define AOVST_L		0x7E
#define AOVST_H		0x7F
#define AOHST_L		0x80
#define AOHST_H		0x81
#define AOVEND_L	0x82
#define AOVEND_H	0x83
#define AOHEND_L	0x84
#define AOHEND_H	0x85
// jitter detecting result
#define JLST_L		0x86
#define JLST_H		0x87
#define JRST_L		0x88
#define JRST_H		0x89
// Auto phase
#define ATPTH		0x8A
#define ATPCTRL		0x8B
#define ATPR_B	BIT1
#define ATPV1		0x8C
#define ATPV2		0x8D
#define ATPV3		0x8E
#define ATPV4		0x8F
// VSync Status
#define ASCTRLL		0x90
#define IVB_B	BIT7
#define LSLVP_L		0x91
#define LSLVP_H		0x92
#define LSLW_L		0x93
#define LSLW_H		0x94
#define LVSST_L		0x95
#define LVSST_H		0x96
#define LHTST_L		0x97
#define LHTST_H		0x98
#define LFRST_L		0x99 // locking fraction status
#define LFRST_H		0x9A
#define LHMARGIN	0x9B // Locking htotal margin
#define LRSV_L		0x9C // Locking read start value
#define LRSV_H		0x9D //
#define LSMARGIN	0x9E // Locking SSC margin
////////////////////////// osd control ///////////////////////////////
// OSD i/o access
#define OSDIOA		0xA0
#define ORBW_B	BIT1 // Enable OSD register burst write
#define DA_B	BIT2 // Enable OSD Display attribute I/O
#define DC_B	BIT3 // Enable OSD Display code I/O
#define RF_B	BIT4 // Enable OSD RAM font I/O
#define CLR_B	BIT6 // OSD clear bit
#define OSBM_B	BIT7 // OSD SRAM I/O burst write
// osd address/data port
#define OSDRA		0xA1
#define OSDRD		0xA2
// osd ram font address/data port
#define RAMFA		0xA3
#define RAMFD		0xA4
// osd display code address/data port
#define DISPCA_L	0xA5
#define DISPCA_H	0xA6
#define DISPCD		0xA7
// osd display attribute address/data port
#define DISPAA_L	0xA8
#define DISPAA_H	0xA9
#define DISPAD		0xAA
#define Palette256A	0xAC
#define Palette256D	0xAD
#define OSDDF           0xAE
#define RAMFA8_B BIT7
#define DISPCD8_B BIT3
// i/o and pwm
// watch dog timer
#define WDTEN		0xB0
#define WDTKEY		0xB1
#define WDTCNT		0xB2
// DDC control
#define DDCCTRL		0xB3
#define DDCEN1		0xB4
#define D_EN_B	BIT7
#define DDC_LAST1	0xB5
#define DDCADDR		0xB6
#define DDCDATA		0xB7
#define DDCEN2		0xB8
#define DDC_LAST2	0xB9
#define DDCADDR2	0xBA
#define DDCDATA2	0xBB
// output and pwm control
#define MISCFC		0xBC
#define GOUTV		0xBD
#if MST_MCU
    #define PWMDIV0 		0xC2
    #define PWM0C			0xC3
    #define PWMDIV1		0xC4
    #define PWM1C			0xC5
    #define PWMDIV2		0xC6
    #define PWM2C			0xC7
    #define PWMEXT		0xC8
    #define PWMS   		BIT0
    #define PWM0BIT8	BIT1
    #define PWM1BIT8	BIT2
    #define PWM2BIT8	BIT3
    #define PWM3BIT8	BIT6
    #define PWM6    	0xCA
	#define PWM6BIT8	BIT0
	#define PWM6BIT9	BIT1
#else 
    // pwm control
    #define PWMCLK		0xC2
    #define PWM0C		0xC3
    //#define PWM1C		0xC4
    #define PWM1C		0xC5  //2006-04-24 Andy
    #define PWM0F		0xC5
    #define PWM1F		0xC6
    #define PWM2C		0xC7
    #define PWM2F		0xC8
#endif 
// interrupt control
#define INTCTRL		0xCA
#define DCMD	            BIT6 // DVI Clock Missing detected (1 is missing)
#define INTPULSE    	0xCB
// intput status
#define INSTA		0xCC
#define INSTB		0xCD
#define INTENA		0xCE
#define INTENB		0xCF
// Clock generator
#define PLLCTRL1	0xD0
#define BPM_B	BIT3
#define EOCK_B	BIT6
#define XOUT_B	BIT7
#define PLLCTRL2	0xD1
#define MP_PD_B		BIT0 // Master PLL Power down
#define MP_RST_B	BIT1 // Master PLL reset
#define MP_POR_B	BIT2 // Master PLL power on reset
#define MP_K_B		BIT3 // Master PLL Post divider
#define LP_PD_B		BIT4 // output PLL power down
#define LP_RST_B	BIT5 // output PLL reset
#define LP_POR_B	BIT6 // output PLL power on reset
#define MPLL_M		0xD2
#define LPLL_M		0xD3
#define LPLL_CTL2	0xD4
#define LPLL_SET_L	0xD5
#define LPLL_SET_M	0xD6
#define LPLL_SET_H	0xD7
#define LPLL_STEP_L	0xD8
#define LPLL_STEP_H	0xD9
#define LPLL_SPAN_L	0xDA
#define LPLL_SPAN_H	0xDB
// mode detect status
#define STATUS1		0xE0
#define STATUS2		0xE1
#define INTM_B	BIT3
#define SOGD_B	BIT4
#define CSD_B	BIT5
#define SOGP_B	BIT6
#define CSP_B	BIT7
#define VTOTAL_L	0xE2
#define VTOTAL_H	0xE3
#define HSPRD_L		0xE4
#define HSPRD_H		0xE5
#define IHDM_B	BIT7
// Sync Change tolerance
#define HSTOL		0xE6
#define VSTOL		0xE7
// status override/interlace detect
#define ISOVRD		0xE8
#define MDCTRL		0xE9
#define SOGHSPW		0xEA // SOG HSync Pulse width (RO)
#define DVICKTH		0xEB
//------------------- Misc control (13, EEh-F6h)
// Coast control
#define COCTRL1		0xED
#define CTA_B	BIT0 // Eanble Coast
#define COVS_B	BIT1 // COAST VSYNC SELECT
#define EXVS_B	BIT2 // External Vsync Polarity
#define CSCM_B	BIT3 // Enalbe Composite Sync cut mode
#define DLYV_B	BIT4 // Analog Delay line for YCbCr
#define AVIS_B	BIT5 // Analog Video input select
#define COCTRL2		0xEE
#define COCTRL3		0xEF
// Power down control/ software reset
#define PDMD		0xF0
#define PDDS_B	BIT4
#define VDD2LOW	BIT6// 2005/5/12 ¤W¤È 10:33:37 by keng
#define SWRST		0xF1
#define SWR_B	BIT0
#define OSDR_B	BIT1
#define BIUR_B	BIT2
#define DPR_B	BIT3
#define GPR_B	BIT4
#define ADCR_B	BIT5
// output signal control
#define OSCTRL		0xF2
#define OHS_B	BIT0
#define OVS_B	BIT1
#define ODE_B	BIT2
#define OCLK_B	BIT3
// intput signal contorl
#define ISCTRL		0xF3
#define SCKI_B	BIT0	// input sample clock invert
#define CSDM_B	BIT1	// composite syn detect mode
#define ISSM_B	BIT2	// input sync sample mode
#define HSFL_B	BIT3	// input hsync filter
#define DEGE_B	BIT7	// enable DE Glitch
// output tri-state control
#define TRISTATE	0xF4
// output driving current control
#define ODRV		0xF5
// Even clock delay control
#define ECLKDLY		0xF6
#define TEST		0xF8

#define DIMD        0xFA

// chip ID
#define CHIPID		0xFE
//---------------------------OSD register-------------------
// osd double buffer control
#define OSDDBC		0x01
// osd start position
#define OHSTA_L		0x02
#define OHSTA_H		0x03
#define OVSTA_L		0x04
#define OVSTA_H		0x05
// osd size controol
#define OSDW		0x06
#define OSDH		0x07
// osd space control
#define OHSPA		0x08
#define OVSPA		0x09
#define OSPW		0x0A
#define OSPH		0x0B
// internal osd control
#define IOSDC1		0x0C
#define MWIN_B	BIT0
#define Rotate	BIT1	  //071226
#define MWBT_B	BIT3
#define IOSDC2		0x0D
#define IOSDC3		0x0E
//osd windodw shadow control
#define OSDHC		0x0F
#define OCFF		0x10
#define OSDCFA		0x11
//osd code buffer offset/ base address
#define OCBUFO		0x12
#define OSDBA_L		0x13
#define OSDBA_H		0x14
// osd gradually color control
#define GCCTRL		0x15
#define GRADCLR		0x16
// osd horizontal gradullay color
#define HGRADCR		0x17
#define HGRADCG		0x18
#define HGRADCB		0x19
#define HGRADSR		0x1A
#define HGRADSG		0x1B
#define HGRADSB		0x1C
// osd vertical gradullay color
#define VGRADCR		0x1D
#define VGRADCG		0x1E
#define VGRADCB		0x1F
#define VGRADSR		0x20
#define VGRADSG		0x21
#define VGRADSB		0x22
// osd sub window 0 control
#define SUBW0C		0x23
#define SW0HST		0x24
#define SW0HEND		0x25
#define SW0VST		0x26
#define SW0VEND		0x27
#define SUBW0A		0x28
// osd sub window 1 control
#define SUBW1C		0x29
#define SW1HST		0x2A
#define SW1HEND		0x2B
#define SW1VST		0x2C
#define SW1VEND		0x2D
#define SUBW1A		0x2E
// osd sub window 2 control
#define SUBW2C		0x2F
#define SW2HST		0x30
#define SW2HEND		0x31
#define SW2VST		0x32
#define SW2VEND		0x33
#define SUBW2A		0x34
// osd sub window 3 control
#define SUBW3C		0x35
#define SW3HST		0x36
#define SW3HEND		0x37
#define SW3VST		0x38
#define SW3VEND		0x39
#define SUBW3A		0x3A
#define OSD8CFFA	0x3B
#define OSD8CFCA	0x3C
#define OSDCFHA		0x41
// osd row attribute
#define ROW00A		0x3B
#define ROW01A		0x3C
#define ROW02A		0x3D
#define ROW03A		0x3E
#define ROW04A		0x3F
#define ROW05A		0x40
#define ROW06A		0x41
#define ROW07A		0x42
#define ROW08A		0x43
#define ROW09A		0x44
#define ROW0AA		0x45
#define ROW0BA		0x46
#define ROW0CA		0x47
#define ROW0DA		0x48
#define ROW0EA		0x49
#define OSD_Reg4A	0x4A
	#define Font_Cascade	 BIT7
// osd color palette
#define CLR0R		0x58
#define CLR0G		0x59
#define CLR0B		0x5A
#define CLR1R		0x5B
#define CLR1G		0x5C
#define CLR1B		0x5D
#define CLR2R		0x5E
#define CLR2G		0x5F
#define CLR2B		0x60
#define CLR3R		0x61
#define CLR3G		0x62
#define CLR3B		0x63
#define CLR4R		0x64
#define CLR4G		0x65
#define CLR4B		0x66
#define CLR5R		0x67
#define CLR5G		0x68
#define CLR5B		0x69
#define CLR6R		0x6A
#define CLR6G		0x6B
#define CLR6B		0x6C
#define CLR7R		0x6D
#define CLR7G		0x6E
#define CLR7B		0x6F

#define OSD_GD          0x81
#define OSD_ColorGD	0x77
#define OSD_GD00	0x78
#define OSD_GD01      0x85
#define OSD_GD02      0x92
#define OSD_GD03      0x9F

// Bank =0x10  TCON bank
//Output format control
#define OFC1		0x02
#define OFC2		0x03
#define ESPP_B	BIT7
#define OSPP_B	BIT3
// output drive/polarity control
#define ODPC		0x04
#define ODC		0x05

  #define RSAMLSW_B	BIT0//Amy2007.12.26   20:57:44
  #define	RSBMLSW_B	BIT1
// input format control
#define IFCTRL		0x07
#define CLKB_B	BIT0 // clock blank when GPOA is low
#define SPB_B	BIT1 // Start pulse blank when GPOA is low
#define POL_B	BIT2 // POL blank when GPOA is low
#define DATI_B	BIT3 // data inver
#define G0AT_B	BIT5 // GPO0 auto toggle
#define PUA_B	BIT6
//GPO0(OPOL)
#define G0VST_L		0x08
#define G0VST_H		0x09
#define G0VEND_L	0x0A
#define G0VEND_H	0x0B
#define G0HST_L		0x0C
#define G0HST_H		0x0D
#define G0HEND_L	0x0E
#define G0HEND_H	0x0F
#define G0CTRL		0x10
//GPO1(EPOL)
#define G1VST_L		0x11
#define G1VST_H		0x12
#define G1VEND_L	0x13
#define G1VEND_H	0x14
#define G1HST_L		0x15
#define G1HST_H		0x16
#define G1HEND_L	0x17
#define G1HEND_H	0x18
#define G1CTRL		0x19
//GPO2(RSP2)
#define G2VST_L		0x1A
#define G2VST_H		0x1B
#define G2VEND_L	0x1C
#define G2VEND_H	0x1D
#define G2HST_L		0x1E
#define G2HST_H		0x1F
#define G2HEND_L	0x20
#define G2HEND_H	0x21
#define G2CTRL		0x22
//GPO3(RSP3)
#define G3VST_L		0x23
#define G3VST_H		0x24
#define G3VEND_L	0x25
#define G3VEND_H	0x26
#define G3HST_L		0x27
#define G3HST_H		0x28
#define G3HEND_L	0x29
#define G3HEND_H	0x2A
#define G3CTRL		0x2B
//GPO4(RCLK)
#define G4VST_L		0x2C
#define G4VST_H		0x2D
#define G4VEND_L	0x2E
#define G4VEND_H	0x2F
#define G4HST_L		0x30
#define G4HST_H		0x31
#define G4HEND_L	0x32
#define G4HEND_H	0x33
#define G4CTRL		0x34
//GPO5(ROE)
#define G5VST_L		0x35
#define G5VST_H		0x36
#define G5VEND_L	0x37
#define G5VEND_H	0x38
#define G5HST_L		0x39
#define G5HST_H		0x3A
#define G5HEND_L	0x3B
#define G5HEND_H	0x3C
#define G5CTRL		0x3D
//GPO6(ROE2)
#define G6VST_L		0x3E
#define G6VST_H		0x3F
#define G6VEND_L	0x40
#define G6VEND_H	0x41
#define G6HST_L		0x42
#define G6HST_H		0x43
#define G6HEND_L	0x44
#define G6HEND_H	0x45
#define G6CTRL		0x46
//GPO7(ROE3)
#define G7VST_L		0x47
#define G7VST_H		0x48
#define G7VEND_L	0x49
#define G7VEND_H	0x4A
#define G7HST_L		0x4B
#define G7HST_H		0x4C
#define G7HEND_L	0x4D
#define G7HEND_H	0x4E
#define G7CTRL		0x4F
//GPO8(DHS/TCON_LP)
#define G8VST_L		0x50
#define G8VST_H		0x51
#define G8VEND_L	0x52
#define G8VEND_H	0x53
#define G8HST_L		0x54
#define G8HST_H		0x55
#define G8HEND_L	0x56
#define G8HEND_H	0x57
#define G8CTRL		0x58
//GPO9(DVS/TCON_FSYNC)
#define G9VST_L		0x59
#define G9VST_H		0x5A
#define G9VEND_L	0x5B
#define G9VEND_H	0x5C
#define G9HST_L		0x5D
#define G9HST_H		0x5E
#define G9HEND_L	0x5F
#define G9HEND_H	0x60
#define G9CTRL		0x61
//GPOA
#define GAVST_L		0x62
#define GAVST_H		0x63
#define GAVEND_L	0x64
#define GAVEND_H	0x65
#define GAHST_L		0x66
#define GAHST_H		0x67
#define GAHEND_L	0x68
#define GAHEND_H	0x69
#define GACTRL		0x6A
// TV Register Mapping
#define BK0_00  			0x00
#define REG_BANK_MASK		0x07     //
#define REG_BANK_SCALER     	0x00     // Scaler bank
#define REG_BANK_ADC_DVI    0x01     // ADC/DVI receiver bank
#define REG_BANK_TCON       	0x02     // Timing control bank
#define REG_BANK_PIP        	0x03     // Video control bank
#define REG_BANK_MIU        	0x04     // Video & MIU control bank
///////////////////////////////////////////////////////
// [Bank = 001]
// ADC/DVI receiver bank
///////////////////////////////////////////////////////
// Double buffer control
#define BK1_01  0x01 // enable dobule buffer while vertical blanking
// PLL divider control
#define BK1_02  0x02 // PLL divider ratio (horizontal total)
#define BK1_03  0x03
// Input R/G/B Gain
#define BK1_04  0x04
#define BK1_05  0x05
#define BK1_06  0x06
// Input R/G/B Offset
#define BK1_07  0x07
#define BK1_08  0x08
#define BK1_09  0x09
// Clamp timing
#define BK1_0A  0x0A // clamping placement based on ADC clock
#define BK1_0B  0x0B // clamping duration based on ADC clock
// General control
#define BK1_0C  0x0C
// PLL coefficient
#define BK1_0D  0x0D // PLL loop filter control
#define BK1_0E  0x0E
#define BK1_0F  0x0F
// Clock phase control
#define BK1_10  0x10 // b5-0: clock phase adjust(PHASECC + 8)
#define BK1_11  0x11 // b5-0: PHASECC(clock phase adjust)
// VCO control
#define BK1_12  0x12
#define BK1_13  0x13
// SOG/HSYNC programming level
#define BK1_14  0x14
#define BK1_15  0x15
// Status
#define BK1_16  0x16 //
#define BK1_18  0x18 // DVI termination resistor status in 2's complement(DVI feature only)
#define BK1_19  0x19 // DVI phase status indicator in 2's complement(DVI feature only)
#define BK1_1A  0x1A
// DVI override(DVI feature only)
#define BK1_1B  0x1B // freeze & override DVI red channel PLL phase selection
#define BK1_1C  0x1C // freeze & override DVI green channel PLL phase selection
#define BK1_1D  0x1D // freeze & override DVI blue channel PLL phase selection
#define BK1_1E  0x1E // DVI bit error status indicator
#define BK1_1F  0x1F // DVI bit error tolerance threshold
#define BK1_20  0x20
// b5-4: channel select for DVI error status indicator(DVI feature only)
#define DVI_RED_CHANNEL     0x00
#define DVI_GREEN_CHANNEL   0x10
#define DVI_BLUE_CHANNEL    0x20
#define BK1_21  0x21
#define BK1_22  0x22
#define BK1_23  0x23
#define BK1_24  0x24
#define BK1_25  0x25
#define BK1_26  0x26
#define BK1_27  0x27
#define BK1_28  0x28
#define BK1_29  0x29
#define BK1_2A  0x2A
#define BK1_2B  0x2B
#define BK1_2C  0x2C
#define BK1_2D  0x2D
#define BK1_2E  0x2E
#define BK1_2F  0x2F
// PLL control for video input
#define BK1_30  0x30
#define BK1_31  0x31
#define BK1_32  0x32
#define BK1_33  0x33
#define BK1_34  0x34
#define BK1_35  0x35
#define BK1_36  0x36
#define BK1_37  0x37
#define BK1_38  0x38
#define BK1_3C  0x3C
#define BK1_3D  0x3D
#define BK1_3E  0x3E
#define BK1_3F  0x3F

#define BK1_40  0x40
#define BK1_41  0x41
#define BK1_42  0x42
#define BK1_43  0x43
#define BK1_44  0x44
#define BK1_45  0x45
#define BK1_46  0x46
#define BK1_47  0x47
#define BK1_48  0x48
#define BK1_49  0x49
#define BK1_4A  0x4A
#define BK1_4B  0x4B
#define BK1_4C  0x4C
#define BK1_4D  0x4D
#define BK1_4E  0x4E
#define BK1_4F  0x4F

#define BK1_50  0x50
#define BK1_51  0x51
#define BK1_52  0x52
#define BK1_53  0x53
#define BK1_54  0x54
#define BK1_55  0x55
#define BK1_56  0x56
#define BK1_57  0x57
#define BK1_58  0x58
#define BK1_59  0x59
#define BK1_5A  0x5A
#define BK1_5B  0x5B
#define BK1_5C  0x5C
#define BK1_5D  0x5D
#define BK1_5E  0x5E
#define BK1_5F  0x5F

#define BK1_60  0x60
#define BK1_61  0x61
#define BK1_62  0x62
#define BK1_63  0x63
#define BK1_64  0x64
#define BK1_66  0x66
#define BK1_67  0x67
#define BK1_68  0x68
#define BK1_69  0x69

#define BK1_79  0x79

// HDMI status registers
#define BK1_80  0x80
#define BK1_81  0x81
// HDMI error registers
#define BK1_83  0x83
#define BK1_84  0x84
// HDMI config registers
#define BK1_86  0x86
#define BK1_87  0x87
#define BK1_88  0x88
// HDMI test Mux control registers
#define BK1_8A  0x8A
// HDMI audio PLL control registers
#define BK1_8C  0x8C
#define BK1_8D  0x8D     // audio PLL test controll [7:0]
#define BK1_8E  0x8E     // audio PLL test controll [15:8]
// HDMI frequency compare registers
#define BK1_8F  0x8F     // 50M count value [7:0]
#define BK1_90  0x90     // 50M count value [15:8]
#define BK1_91  0x91     // 100M count value [7:0]
#define BK1_92  0x92     // 100M count value [15:8]
#define BK1_93  0x93     // 200M count value [7:0]
#define BK1_94  0x94     // 200M count value [15:8]
// HDMI packet type registers
#define BK1_95  0x95
// HDMI pixel clock frquency registers
#define BK1_98  0x98     // counter outputs plus overflow [7:0]
#define BK1_99  0x99
// HDMI audio clock registers
#define BK1_9A  0x9A // CTS for audio clock regeneration [7:0]
#define BK1_9B  0x9B // CTS for audio clock regeneration [15:8]
#define BK1_9C  0x9C // CTS for audio clock regeneration [19:16]
#define BK1_9D  0x9D // N for audio clock regeneration [7:0]
#define BK1_9E  0x9E // N for audio clock regeneration [15:8]
#define BK1_9F  0x9F // N for audio clock regeneration [19:16]
// HDMI general control registers
#define BK1_A0  0xA0
// HDMI AVI IF registers
#define BK1_A1  0xA1
#define BK1_A2  0xA2
#define BK1_A3  0xA3
#define BK1_A4  0xA4
#define BK1_A5  0xA5 // line number of end of top bar [7:0]
#define BK1_A6  0xA6 // line number of end of top bar [15:8]
#define BK1_A7  0xA7 // line number of start of bottom bar [7:0]
#define BK1_A8  0xA8 // line number of start of bottom bar [15:8]
#define BK1_A9  0xA9 // pixel number of end of left bar [7:0]
#define BK1_AA  0xAA // pixel number of end of left bar [15:8]
#define BK1_AB  0xAB // pixel number of start of right bar [7:0]
#define BK1_AC  0xAC // pixel number of start of right bar [15:8]
// HDMI audio IF registers
#define BK1_AD  0xAD
#define BK1_AE  0xAE
#define BK1_AF  0xAF // max bit rate depending on AUD_CT
#define BK1_B0  0xB0 // channel allocation
#define BK1_B1  0xB1
// HDMI SPD IF registers
#define BK1_B2  0xB2 // b6-0: (SPD_VN1) vendor name character 1
#define BK1_B3  0xB3 // b6-0: (SPD_VN2) vendor name character 2
#define BK1_B4  0xB4 // b6-0: (SPD_VN3) vendor name character 3
#define BK1_B5  0xB5 // b6-0: (SPD_VN4) vendor name character 4
#define BK1_B6  0xB6 // b6-0: (SPD_VN5) vendor name character 5
#define BK1_B7  0xB7 // b6-0: (SPD_VN6) vendor name character 6
#define BK1_B8  0xB8 // b6-0: (SPD_VN7) vendor name character 7
#define BK1_B9  0xB9 // b6-0: (SPD_VN8) vendor name character 8
#define BK1_BA  0xBA // b6-0: (SPD_PD1) product description character 1
#define BK1_BB  0xBB // b6-0: (SPD_PD2) product description character 2
#define BK1_BC  0xBC // b6-0: (SPD_PD3) product description character 3
#define BK1_BD  0xBD // b6-0: (SPD_PD4) product description character 4
#define BK1_BE  0xBE // b6-0: (SPD_PD5) product description character 5
#define BK1_BF  0xBF // b6-0: (SPD_PD6) product description character 6
#define BK1_C0  0xC0 // b6-0: (SPD_PD7) product description character 7
#define BK1_C1  0xC1 // b6-0: (SPD_PD8) product description character 8
#define BK1_C2  0xC2 // b6-0: (SPD_PD9) product description character 9
#define BK1_C3  0xC3 // b6-0: (SPD_PD10) product description character 10
#define BK1_C4  0xC4 // b6-0: (SPD_PD11) product description character 11
#define BK1_C5  0xC5 // b6-0: (SPD_PD12) product description character 12
#define BK1_C6  0xC6 // b6-0: (SPD_PD13) product description character 13
#define BK1_C7  0xC7 // b6-0: (SPD_PD14) product description character 14
#define BK1_C8  0xC8 // b6-0: (SPD_PD15) product description character 15
#define BK1_C9  0xC9 // b6-0: (SPD_PD16) product description character 16
#define BK1_CA  0xCA // (SPD_SDI) source device information
// HDMI MPEG IF registers
#define BK1_CB  0xCB // (MPEG_MB) MPEG bit rate [7:0]
#define BK1_CC  0xCC // (MPEG_MB) MPEG bit rate [15:8]
#define BK1_CD  0xCD // (MPEG_MB) MPEG bit rate [23:16]
#define BK1_CE  0xCE // (MPEG_MB) MPEG bit rate [31:24]
#define BK1_CF  0xCF
// HDMI vendor specific IF registers
#define BK1_D0  0xD0 // vendor specific InfoFrame length
#define BK1_D1  0xD1 // payload byte 0
#define BK1_D2  0xD2 // payload byte 1
#define BK1_D3  0xD3 // payload byte 2
#define BK1_D4  0xD4 // payload byte 3
#define BK1_D5  0xD5 // payload byte 4
#define BK1_D6  0xD6 // payload byte 5
#define BK1_D7  0xD7 // payload byte 6
#define BK1_D8  0xD8 // payload byte 7
#define BK1_D9  0xD9 // payload byte 8
#define BK1_DA  0xDA // payload byte 9
#define BK1_DB  0xDB // payload byte 10
#define BK1_DC  0xDC // payload byte 11
#define BK1_DD  0xDD // payload byte 12
#define BK1_DE  0xDE // payload byte 13
#define BK1_DF  0xDF // payload byte 14
#define BK1_E0  0xE0 // payload byte 15
#define BK1_E1  0xE1 // payload byte 16
#define BK1_E2  0xE2 // payload byte 17
#define BK1_E3  0xE3 // payload byte 18
#define BK1_E4  0xE4 // payload byte 19
#define BK1_E5  0xE5 // payload byte 20
#define BK1_E6  0xE6 // payload byte 21
#define BK1_E7  0xE7 // payload byte 22
#define BK1_E8  0xE8 // payload byte 23
#define BK1_E9  0xE9 // payload byte 24
#define BK1_EA  0xEA // payload byte 25
#define BK1_EB  0xEB // payload byte 26
#define BK1_EC  0xEC // payload byte 27
///////////////////////////////////////////////////////
// [Bank = 000]
// Scaler register
///////////////////////////////////////////////////////
// double buffer control
#define BK0_01  0x01
// [Graphic port register]
// input ssource select/control
#define BK0_02  0x02
// b1-0: input select
#define SCALER_ISEL_MASK    0x07
#define SCALER_ISEL_ANALOG1 0x00
#define SCALER_ISEL_ANALOG2 0x01
#define SCALER_ISEL_ANALOG3 0x02
#define SCALER_ISEL_DVI     0x03
#define SCALER_ISEL_VIDEO   0x04
#define SCALER_ISEL_HDMI    0x07
// b6-5: input sync type
#define SCALER_STYPE_SOG    0x60
#define BK0_03  0x03
#define BK0_04  0x04
// input image sample range
#define BK0_05  0x05 // image vertical sample start point, count by input HSync
#define BK0_06  0x06
#define BK0_07  0x07 // image horizontal sample start point, count by input dot clock
#define BK0_08  0x08
#define BK0_09  0x09 // image vertical resolution(vertical display enable area count by line)
#define BK0_0A  0x0A
#define BK0_0B  0x0B // image horizontal resolution(horizontal display enable area count by pixel)
#define BK0_0C  0x0C
// digital video input control
#define BK0_0D  0x0D
// input lock pointer
#define BK0_0F  0x0F // b3-0: lock Y line
// [display timing register]
// ouput DE size
#define BK0_10  0x10 // output DE vertical start
#define BK0_11  0x11
#define BK0_12  0x12 // oouput DE horizontal start
#define BK0_13  0x13
#define BK0_14  0x14 // output DE vertical end
#define BK0_15  0x15
#define BK0_16  0x16 // oouput DE horizontal end
#define BK0_17  0x17
// scaling image window size
#define BK0_18  0x18 // scaling image window horizontal start
#define BK0_19  0x19
#define BK0_1A  0x1A // scaling image window vertical end
#define BK0_1B  0x1B
#define BK0_1C  0x1C // scaling image window horizontal end
#define BK0_1D  0x1D
// output sync timing
#define BK0_1E  0x1E // ouput vertical total
#define BK0_1F  0x1F
#define BK0_20  0x20 // ouput VSync start
#define BK0_21  0x21
#define BK0_22  0x22 // ouput VSync end
#define BK0_23  0x23
#define BK0_24  0x24 // ouput horizontal total
#define BK0_25  0x25
#define BK0_26  0x26 // output HSync pulse width
// output sync control
#define BK0_27  0x27
#define BK0_28  0x28
// Mode 3 control
#define BK0_29  0x29 // mode 3 lock point [7:0]
#define BK0_2A  0x2A // b2-0: mode 3 lock point [10:8]
#define BK0_2B  0x2B // small lock range
#define BK0_2C  0x2C
#define BK0_2D  0x2D // mode 3 VTOTAL [7:0]
#define BK0_2E  0x2E // b2-0: mode 3 VTOTAL [10:8]
#define BK0_2F  0x2F
#define BK0_30  0x30 // mode 3 error [7:0]
#define BK0_31  0x31 // b2-0: mode 3 error [10:8]
// border color
#define BK0_32  0x32
#define BK0_33  0x33 // red
#define BK0_34  0x34 // green
#define BK0_35  0x35 // blue
// dither control
#define BK0_36  0x36
#define BK0_37  0x37 // dither coefficient
#define BK0_38  0x38
// peaking control
#define BK0_3B  0x3B
#define BK0_3C  0x3C
#define BK0_3D  0x3D
#define BK0_3E  0x3E
#define BK0_3F  0x3F
// gamma control
#define BK0_40  0x40
#define BK0_41  0x41 // gamma data port
// output control
#define BK0_42  0x42
#define BK0_43  0x43
#define BK0_44  0x44
// frame rate convert for fail-safe mode / LVDS/RSDS test mode data
#define BK0_47  0x47
// short-line tuning
#define BK0_48  0x48
#define BK0_49  0x49
#define BK0_4A  0x4A
// [OSD overlay register]
// OSD alpha blending control
#define BK0_4B  0x4B // b6-3: color index of color key
// b2-0: alpha blending mode
#define BK0_4C  0x4C // b2-0: OSD alpha blending level
// scaling ratio
#define BK0_50  0x50 // horizontal scaling factor
#define BK0_51  0x51
#define BK0_52  0x52
#define BK0_53  0x53 // vertical scaling factor
#define BK0_54  0x54
#define BK0_55  0x55
// scaling filter control
#define BK0_56  0x56 // horizontal scaling filter
#define BK0_57  0x57 // vertical scaling filter
#define BK0_58  0x58 // horizontal DSUS scaling parameter
#define BK0_59  0x59
#define BK0_5A  0x5A // vertical DSUS scaling parameter
#define BK0_5B  0x5B
#define BK0_5C  0x5C
// color matrix coefficient
#define BK0_5E  0x5E // coefficient 11
#define BK0_5F  0x5F
#define BK0_60  0x60 // coefficient 12
#define BK0_61  0x61
#define BK0_62  0x62 // coefficient 13
#define BK0_63  0x63
#define BK0_64  0x64 // coefficient 21
#define BK0_65  0x65
#define BK0_66  0x66 // coefficient 22
#define BK0_67  0x67
#define BK0_68  0x68 // coefficient 23
#define BK0_69  0x69
#define BK0_6A  0x6A // coefficient 31
#define BK0_6B  0x6B
#define BK0_6C  0x6C // coefficient 32
#define BK0_6D  0x6D
#define BK0_6E  0x6E // coefficient 33
#define BK0_6F  0x6F
// color matrix control
#define BK0_70  0x70
// brightness control
#define BK0_71  0x71
#define BK0_72  0x72
#define BK0_73  0x73
#define BK0_74  0x74
// interlaced mode line shift
#define BK0_75  0x75
// auto gain function adjust
#define BK0_78  0x78 // auto gain function control
#define BK0_79  0x79 // auto gain status
// auto gain value select
#define BK0_7A  0x7A
// auto position function and jitter function control
#define BK0_7B  0x7B
#define BK0_7C  0x7C // auto position valid data value
#define BK0_7D  0x7D // auto gain value
// auto position function result
#define BK0_7E  0x7E  // auto position detected result vertical starting point
#define BK0_7F  0x7F
#define BK0_80  0x80  // auto position detected result horizontal starting point
#define BK0_81  0x81
#define BK0_82  0x82  // auto position detected result vertical end point
#define BK0_83  0x83
#define BK0_84  0x84  // auto position detected result horizontal end point
#define BK0_85  0x85
// jitter function detecting result
#define BK0_86  0x86 // jitter function detected left/right most point state(previous frame)
#define BK0_87  0x87
// auto noise reduction function
#define BK0_88  0x88
// auto phase and detecting result
#define BK0_89  0x89
#define BK0_8A  0x8A // auto phase text threshold for BK0_8F
#define BK0_8B  0x8B
// auto phase value
#define BK0_8C  0x8C
#define BK0_8D  0x8D
#define BK0_8E  0x8E
#define BK0_8F  0x8F
// VSync status
#define BK0_90  0x90
#define BK0_91  0x91 // locking short line vertical position
#define BK0_92  0x92
#define BK0_93  0x93 // locking short line width
#define BK0_94  0x94
#define BK0_95  0x95
#define BK0_96  0x96
#define BK0_97  0x97
#define BK0_98  0x98
#define BK0_99  0x99 // locking fraction status
#define BK0_9A  0x9A
#define BK0_9B  0x9B // locking htotal margin
#define BK0_9C  0x9C // locking read start value
#define BK0_9D  0x9D //
#define BK0_9E  0x9E // locking ssc margin
#define BK0_9F  0x9F
// [ OSD control ]
// OSD I/O access
#define BK0_A0  0xA0
// OSD address/data port
#define BK0_A1  0xA1
#define BK0_A2  0xA2
// OSD RAM font address/data port
#define BK0_A3  0xA3
#define BK0_A4  0xA4
// OSD display code address/data port
#define BK0_A5  0xA5
#define BK0_A6  0xA6
#define BK0_A7  0xA7
// OSD display attribute address/data port
#define BK0_A8  0xA8
#define BK0_A9  0xA9
#define BK0_AA  0xAA
#define BK0_AB  0xAB
#define BK0_AC  0xAC
#define BK0_AD  0xAD
#define BK0_AE  0xAE
#define BK0_AF  0xAF
// [I/O and PWM]
// watch dog timer
#define BK0_B0  0xB0
#define BK0_B1  0xB1
#define BK0_B2  0xB2
// DDC control
#define BK0_B3  0xB3
#define BK0_B4  0xB4
#define BK0_B5  0xB5
#define BK0_B6  0xB6
#define BK0_B7  0xB7
#define BK0_B8  0xB8
#define BK0_B9  0xB9
#define BK0_BA  0xBA
#define BK0_BB  0xBB
// output and PWM control
#define BK0_BC  0xBC
// HDCP control
#define HDCPCTRL 0xBD///BK0_BD  
#define HDCPADR 0xBE///BK0_BE  
#define HDCPDAT 0xBF///BK0_BF  
#define BK0_BD 0xBD///BK0_BD  
#define BK0_BE 0xBE///BK0_BE  
#define BK0_BF 0xBF///BK0_BF  
// DVI DPMS control (only valid when F0h[10]=2'b10)
#define BK0_C0  0xC0
#define BK0_C1  0xC1
// PWM control
#define BK0_C2  0xC2
#define BK0_C3  0xC3
#define BK0_C4  0xC4
#define BK0_C5  0xC5
#define BK0_C6  0xC6
#define BK0_C7  0xC7
// interrupt control
#define BK0_CA  0xCA
#define BK0_CB  0xCB // interrupt pulse width by reference click
// intput status
#define BK0_CC  0xCC // input VSync changed
#define BK0_CD  0xCD // input HSync changed
#define BK0_CE  0xCE // input VSync disappear
#define BK0_CF  0xCF // input HSync disappear
// clock generator
#define BK0_D0  0xD0
#define BK0_D1  0xD1
// master & output PLL control
#define BK0_D2  0xD2 // master PLL divider
#define BK0_D3  0xD3
#define BK0_D4  0xD4
// frequency synthesizer & SSC control
#define BK0_D5  0xD5 // output PLL set
#define BK0_D6  0xD6
#define BK0_D7  0xD7
#define BK0_D8  0xD8 // output PLL spread spectrum step
#define BK0_D9  0xD9
#define BK0_DA  0xDA // output PLL spread spectrum span
#define BK0_DB  0xDB
// PLL test control
#define BK0_DC  0xDC
#define BK0_DD  0xDD
#define BK0_DE  0xDE
#define BK0_DF  0xDF
// [Mode detect status]
#define BK0_E0  0xE0 // input sync monitor
#define BK0_E1  0xE1
#define BK0_E2  0xE2 // input vertical total, count by HSync
#define BK0_E3  0xE3
#define BK0_E4  0xE4 // input horizontal period, count by reference clock
#define BK0_E5  0xE5
// Sync change tolerance
#define BK0_E6  0xE6 // Hsync tolerance
#define BK0_E7  0xE7 // VSync tolerance
// status override/interlace detect
#define BK0_E8  0xE8
#define BK0_E9  0xE9
// SOG HSYNC pulse width (when scaler bank 0x02[1:0] <> 2'b10 (not DVI input))
// DVI low clock frequency detection (when scaler bank 0x02[1:0] = 2'b10 (DVI feature only))
#define BK0_EA  0xEA
#define BK0_EB  0xEB
// [Misc. control]
// coast control
#define BK0_ED  0xED
#define BK0_EE  0xEE // end tuning
#define BK0_EF  0xEF // front tuning
// power down control/ software reset
#define BK0_F0  0xF0
#define BK0_F1  0xF1
#define SWRST_ALL   0x3f
// output signal control
#define BK0_F2  0xF2
// intput signal contorl
#define BK0_F3  0xF3
// output tri-state control
#define BK0_F4  0xF4
// output driving current control
#define BK0_F5  0xF5
// even clock delay control
#define BK0_F6  0xF6
// test mode control
#define BK0_F8  0xF8
// SRAM test status
#define BK0_F9  0xF9
// chip ID
#define BK0_FD  0xFD // default = 0x00
#define BK0_FE  0xFE // default = 0xF3
///////////////////////////////////////////////////////
// [OSD register]
///////////////////////////////////////////////////////
// OSD double buffer control
#define OSD_01  0x01
// OSD start position
#define OSD_02  0x02 // OSD window horizontal start postion = 4 * OHSTA + 48(pixels)
#define OSD_03  0x03
#define OSD_04  0x04 // OSD window vertical start postion = 4 * OVSTA(lines)
#define OSD_05  0x05
// OSD size controol
#define OSD_06  0x06 // OSD window width = OSD_06 + 1(column), maximum 70 columns
#define OSD_07  0x07 // OSD window height = OSD_07 + 1(row), maximum 64 rows
// OSD space control
#define OSD_08  0x08 // OSD window horizontal space start position = OSD_08 + 1(row)
#define OSD_09  0x09 // OSD window vertical space start position = OSD_09 + 1(column)
#define OSD_0A  0x0A // OSD space width = 8 * OSD_0A(pixel)
#define OSD_0B  0x0B // OSD space height = 8 * OSD_0B(lline)
// internal osd control
#define OSD_0C  0x0C
#define OSD_0D  0x0D
#define OSD_0E  0x0E
// OSD window shadow control
#define OSD_0F  0x0F // OSD shadow height/width

#define OSD_10  0x10

// OSD code buffer offset/ base address
#define OSD_12  0x12 // b7: OSD code buffer offset select
#define OSD_13  0x13 // OSD code base address
#define OSD_14  0x14
// OSD gradually color control
#define OSD_15  0x15
// OSD starting gradually color
#define OSD_16  0x16
// OSD horizontal gradullay color
#define OSD_17  0x17 // increase/decrease value of Red color
#define OSD_18  0x18 // increase/decrease value of Green color
#define OSD_19  0x19 // increase/decrease value of Blue color
#define OSD_1A  0x1A // horizontal gradually strp of Red color
#define OSD_1B  0x1B // horizontal gradually strp of Green color
#define OSD_1C  0x1C // horizontal gradually strp of Blue color
// OSD vertical gradullay color
#define OSD_1D  0x1D // increase/decrease value of Red color
#define OSD_1E  0x1E // increase/decrease value of Green color
#define OSD_1F  0x1F // increase/decrease value of Blue color
#define OSD_20  0x20 // vertical gradually strp of Red color
#define OSD_21  0x21 // vertical gradually strp of Green color
#define OSD_22  0x22 // vertical gradually strp of Blue color
// OSD sub window 0 control
#define OSD_23  0x23
// OSD sub window 0 horizontal/vertical position
#define OSD_24  0x24 // sub window 0 horizontal start position
#define OSD_25  0x25 // sub window 0 horizontal end position
#define OSD_26  0x26 // sub window 0 vertical start position
#define OSD_27  0x27 // sub window 0 vertical end position
#define OSD_28  0x28 // OSD sub window 0 attribute
// OSD sub window 1 control
#define OSD_29  0x29
// OSD sub window 1 horizontal/vertical position
#define OSD_2A  0x2A // sub window 1 horizontal start position
#define OSD_2B  0x2B // sub window 1 horizontal end position
#define OSD_2C  0x2C // sub window 1 vertical start position
#define OSD_2D  0x2D // sub window 1 vertical end position
#define OSD_2E  0x2E // OSD sub window 1 attribute
// OSD sub window 2 control
#define OSD_2F  0x2F
// OSD sub window 2 horizontal/vertical position
#define OSD_30  0x30 // sub window 2 horizontal start position
#define OSD_31  0x31 // sub window 2 horizontal end position
#define OSD_32  0x32 // sub window 2 vertical start position
#define OSD_33  0x33 // sub window 2 vertical end position
#define OSD_34  0x34 // OSD sub window 2 attribute
// OSD sub window 3 control
#define OSD_35  0x35
// OSD sub window 3 horizontal/vertical position
#define OSD_36  0x36 // sub window 3 horizontal start position
#define OSD_37  0x37 // sub window 3 horizontal end position
#define OSD_38  0x38 // sub window 3 vertical start position
#define OSD_39  0x39 // sub window 3 vertical end position
#define OSD_3A  0x3A // OSD sub window 3 attribute
// OSD color key
#define OSD_3D  0x3D
#define OSD_3E  0x3E
#define OSD_3F  0x3F
// OSD button index
#define OSD_40  0x40
// OSD blink speed and scroll function
#define OSD_41  0x41
#define OSD_42  0x42
#define OSD_43  0x43
// OSD CPU write control
#define OSD_70  0x70
// OSD random test pattern
#define OSD_71  0x71
#define OSD_77  0x77

#define OSD_78   0x78
///////////////////////////////////////////////////////
// [Bank = 010]
// TCON register
///////////////////////////////////////////////////////
// output format control(TCON feature only)
#define BK2_02  0x02
#define BK2_03  0x03
// output drive/polarity control
#define BK2_04  0x04
#define BK2_05  0x05
// GPO4 (OE) active delay frame (TCON feature only)
#define BK2_06  0x06
// input format control
#define BK2_07  0x07
// GPO0(OPOL)
#define BK2_08  0x08 // line number that GPO0 start
#define BK2_09  0x09
#define BK2_0A  0x0A // line number that GPO0 end
#define BK2_0B  0x0B
#define BK2_0C  0x0C // pixel number that GPO0 start
#define BK2_0D  0x0D
#define BK2_0E  0x0E // pixel number that GPO0 end
#define BK2_0F  0x0F
#define BK2_10  0x10
// GPO1(EPOL)
#define BK2_11  0x11 // line number that GPO1 start
#define BK2_12  0x12
#define BK2_13  0x13 // line number that GPO1 end
#define BK2_14  0x14
#define BK2_15  0x15 // pixel number that GPO1 start
#define BK2_16  0x16
#define BK2_17  0x17 // pixel number that GPO1 end
#define BK2_18  0x18
#define BK2_19  0x19
// GPO2(RSP2)
#define BK2_1A  0x1A // line number that GPO2 start
#define BK2_1B  0x1B
#define BK2_1C  0x1C // line number that GPO2 end
#define BK2_1D  0x1D
#define BK2_1E  0x1E // pixel number that GPO2 start
#define BK2_1F  0x1F
#define BK2_20  0x20 // pixel number that GPO2 end
#define BK2_21  0x21
#define BK2_22  0x22
// GPO3(RSP3)
#define BK2_23  0x23 // line number that GPO3 start
#define BK2_24  0x24
#define BK2_25  0x25 // line number that GPO3 end
#define BK2_26  0x26
#define BK2_27  0x27 // pixel number that GPO3 start
#define BK2_28  0x28
#define BK2_29  0x29 // pixel number that GPO3 end
#define BK2_2A  0x2A
#define BK2_2B  0x2B
// GPO4(RCLK)
#define BK2_2C  0x2C // line number that GPO4 start
#define BK2_2D  0x2D
#define BK2_2E  0x2E // line number that GPO4 end
#define BK2_2F  0x2F
#define BK2_30  0x30 // pixel number that GPO4 start
#define BK2_31  0x31
#define BK2_32  0x32 // pixel number that GPO4 end
#define BK2_33  0x33
#define BK2_34  0x34
// GPO5(ROE)
#define BK2_35  0x35 // line number that GPO5 start
#define BK2_36  0x36
#define BK2_37  0x37 // line number that GPO5 end
#define BK2_38  0x38
#define BK2_39  0x39 // pixel number that GPO5 start
#define BK2_3A  0x3A
#define BK2_3B  0x3B // pixel number that GPO5 end
#define BK2_3C  0x3C
#define BK2_3D  0x3D
// GPO6(ROE2)
#define BK2_3E  0x3E // line number that GPO6 start
#define BK2_3F  0x3F
#define BK2_40  0x40 // line number that GPO6 end
#define BK2_41  0x41
#define BK2_42  0x42 // pixel number that GPO6 start
#define BK2_43  0x43
#define BK2_44  0x44 // pixel number that GPO6 end
#define BK2_45  0x45
#define BK2_46  0x46
// GPO7(ROE3)
#define BK2_47  0x47 // line number that GPO7 start
#define BK2_48  0x48
#define BK2_49  0x49 // line number that GPO7 end
#define BK2_4A  0x4A
#define BK2_4B  0x4B // pixel number that GPO7 start
#define BK2_4C  0x4C
#define BK2_4D  0x4D // pixel number that GPO7 end
#define BK2_4E  0x4E
#define BK2_4F  0x4F
// GPO8(DHS/TCON_LP)
#define BK2_50  0x50 // line number that GPO8 start
#define BK2_51  0x51
#define BK2_52  0x52 // line number that GPO8 end
#define BK2_53  0x53
#define BK2_54  0x54 // pixel number that GPO8 start
#define BK2_55  0x55
#define BK2_56  0x56 // pixel number that GPO8 end
#define BK2_57  0x57
#define BK2_58  0x58
// GPO9(DVS/TCON_FSYNC)
#define BK2_59  0x59 // line number that GPO9 start
#define BK2_5A  0x5A
#define BK2_5B  0x5B // line number that GPO9 end
#define BK2_5C  0x5C
#define BK2_5D  0x5D // pixel number that GPO9 start
#define BK2_5E  0x5E
#define BK2_5F  0x5F // pixel number that GPO9 end
#define BK2_60  0x60
#define BK2_61  0x61
// GPOA
#define BK2_62  0x62 // line number that GPOA start
#define BK2_63  0x63
#define BK2_64  0x64 // line number that GPOA end
#define BK2_65  0x65
#define BK2_66  0x66 // pixel number that GPOA start
#define BK2_67  0x67
#define BK2_68  0x68 // pixel number that GPOA end
#define BK2_69  0x69
#define BK2_6A  0x6A
#define BK2_6B  0x6B
#define BK2_6C  0x6C // coefficient 32
#define BK2_6D  0x6D
#define BK2_6E  0x6E // coefficient 33
#define BK2_6F  0x6F
#define BK2_70  0x70
#define BK2_71  0x71
#define BK2_72  0x72
#define BK2_73  0x73
#define BK2_74  0x74
#define BK2_75  0x75
#define BK2_76  0x76
#define BK2_77  0x77
#define BK2_78  0x78
#define BK2_79  0x79
#define BK2_7A  0x7A
#define BK2_7B  0x7B
#define BK2_7C  0x7C
#define BK2_7D  0x7D
#define BK2_7E  0x7E
#define BK2_7F  0x7F
#define BK2_80  0x80
#define BK2_81  0x81
#define BK2_82  0x82
#define BK2_83  0x83
#define BK2_84  0x84
#define BK2_85  0x85
#define BK2_86  0x86
#define BK2_87  0x87
#define BK2_88  0x88
#define BK2_89  0x89
#define BK2_8A  0x8A
#define BK2_8B  0x8B
#define BK2_8C  0x8C
#define BK2_8D  0x8D
#define BK2_8E  0x8E
#define BK2_8F  0x8F
#define BK2_90  0x90
#define BK2_91  0x91
#define BK2_92  0x92
#define BK2_93  0x93
#define BK2_94  0x94
#define BK2_95  0x95
#define BK2_96  0x96
#define BK2_97  0x97
#define BK2_98  0x98
#define BK2_99  0x99
#define BK2_9A  0x9A
#define BK2_9B  0x9B
#define BK2_9C  0x9C
#define BK2_9D  0x9D
#define BK2_9E  0x9E
#define BK2_9F  0x9F
#define BK2_A0  0xA0
#define BK2_A1  0xA1
#define BK2_A2  0xA2
#define BK2_A3  0xA3
#define BK2_A4  0xA4
#define BK2_A5  0xA5
#define BK2_A7  0xA7
#define BK2_A8  0xA8
#define BK2_A9  0xA9
#define BK2_AA  0xAA
#define BK2_AB  0xAB
#define BK2_AC  0xAC
#define BK2_AD  0xAD
#define BK2_AE  0xAE
#define BK2_AF  0xAF
#define BK2_B0  0xB0
#define BK2_B1  0xB1
#define BK2_B2  0xB2
#define BK2_B3  0xB3
#define BK2_B4  0xB4
#define BK2_B5  0xB5
#define BK2_B6  0xB6
#define BK2_B7  0xB7
#define BK2_B8  0xB8
#define BK2_B9  0xB9
#define BK2_BA  0xBA
#define BK2_BB  0xBB
#define BK2_BC  0xBC
#define BK2_BD  0xBD
#define BK2_BE  0xBE
#define BK2_BF  0xBF
#define BK2_C0  0xC0
#define BK2_C1  0xC1
#define BK2_C2  0xC2
#define BK2_C3  0xC3
#define BK2_C4  0xC4
#define BK2_C5  0xC5
#define BK2_C6  0xC6
#define BK2_C7  0xC7
#define BK2_C8  0xC8
#define BK2_C9  0xC9
#define BK2_CA  0xCA
#define BK2_CB  0xCB
#define BK2_CC  0xCC
#define BK2_CD  0xCD
#define BK2_CE  0xCE // coefficient 11
#define BK2_CF  0xCF
#define BK2_D0  0xD0 // coefficient 12
#define BK2_D1  0xD1
#define BK2_D2  0xD2 // coefficient 13
#define BK2_D3  0xD3
#define BK2_D4  0xD4 // coefficient 21
#define BK2_D5  0xD5
#define BK2_D6  0xD6 // coefficient 22
#define BK2_D7  0xD7
#define BK2_D8  0xD8 // coefficient 23
#define BK2_D9  0xD9
#define BK2_DA  0xDA // coefficient 31
#define BK2_DB  0xDB
#define BK2_DC  0xDC // coefficient 32
#define BK2_DD  0xDD
#define BK2_DE  0xDE // coefficient 33
#define BK2_DF  0xDF
#define BK2_E0  0xE0 // input sync monitor
#define BK2_E1  0xE1
#define BK2_E2  0xE2 // input vertical total, count by HSync
#define BK2_E3  0xE3
#define BK2_E4  0xE4 // input horizontal period, count by reference clock
#define BK2_E5  0xE5
#define BK2_E6  0xE6 // Hsync tolerance
#define BK2_E7  0xE7 // VSync tolerance
#define BK2_E8  0xE8
#define BK2_E9  0xE9
#define BK2_ED  0xED
#define BK2_EE  0xEE
#define BK2_EF  0xEF
///////////////////////////////////////////////////////
// [Bank = 011]
// MIU/PIP bank
///////////////////////////////////////////////////////
// double buffer control
#define BK3_01  0x01
// input ssource select/control
#define BK3_02  0x02
#define BK3_03  0x03
#define BK3_04  0x04
// input image sample range
#define BK3_05  0x05 // image vertical sample start point, count by input HSync
#define BK3_06  0x06
#define BK3_07  0x07 // image horizontal sample start point, count by input dot clock
#define BK3_08  0x08
#define BK3_09  0x09 // image vertical resolution(vertical display enable area count by line)
#define BK3_0A  0x0A
#define BK3_0B  0x0B // image horizontal resolution(horizontal display enable area count by pixel)
#define BK3_0C  0x0C
// digital video input control
#define BK3_0D  0x0D
// sub-window border size
#define BK3_10  0x10
#define BK3_11  0x11
#define BK3_12  0x12
#define BK3_13  0x13
#define BK3_14  0x14
#define BK3_15  0x15
#define BK3_16  0x16
#define BK3_17  0x17
// sub-window image size
#define BK3_18  0x18
#define BK3_19  0x19
#define BK3_1A  0x1A
#define BK3_1B  0x1B
#define BK3_1C  0x1C
#define BK3_1D  0x1D
#define BK3_1E  0x1E
#define BK3_1F  0x1F
// peaking control
#define BK3_39  0x39
#define BK3_3A  0x3A
#define BK3_3B  0x3B
#define BK3_3C  0x3C
#define BK3_3D  0x3D
#define BK3_3E  0x3E
#define BK3_3F  0x3F
// Gamma control
#define BK3_40  0x40
// PIP registers
// PIP window border color
#define BK3_47  0x47
// PIP control
#define BK3_48  0x48
// scaling function registers
// non-linear scaling control
#define BK3_49  0x49
#define BK3_4A  0x4A
#define BK3_4B  0x4B
#define BK3_4C  0x4C
#define BK3_4D  0x4D
#define BK3_4E  0x4E
#define BK3_4F  0x4F
// scaling ratio
#define BK3_50  0x50 // horizontal scaling factor
#define BK3_51  0x51
#define BK3_52  0x52
#define BK3_53  0x53 // vertical scaling factor
#define BK3_54  0x54
#define BK3_55  0x55
// scaling filter control
#define BK3_56  0x56 // horizontal scaling filter
#define BK3_57  0x57 // vertical scaling filter
#define BK3_5A  0x5A // scaling parameter
// first color matrix coefficient
#define BK3_5E  0x5E // coefficient 11
#define BK3_5F  0x5F
#define BK3_60  0x60 // coefficient 12
#define BK3_61  0x61
#define BK3_62  0x62 // coefficient 13
#define BK3_63  0x63
#define BK3_64  0x64 // coefficient 21
#define BK3_65  0x65
#define BK3_66  0x66 // coefficient 22
#define BK3_67  0x67
#define BK3_68  0x68 // coefficient 23
#define BK3_69  0x69
#define BK3_6A  0x6A // coefficient 31
#define BK3_6B  0x6B
#define BK3_6C  0x6C // coefficient 32
#define BK3_6D  0x6D
#define BK3_6E  0x6E // coefficient 33
#define BK3_6F  0x6F
// color matrix control
#define BK3_70  0x70
// brightness control
#define BK3_71  0x71
#define BK3_72  0x72
#define BK3_73  0x73
#define BK3_74  0x74
// flesh tone detection control
#define BK3_75  0x75
#define BK3_76  0x76
// YC delay control
#define BK3_77  0x77
// NR
#define BK3_7A  0x7A
#define BK3_7B  0x7B
#define BK3_7C  0x7C
#define BK3_7D  0x7D
#define BK3_7E  0x7E
#define BK3_7F  0x7F
// audio noise reduction function
#define BK3_80  0x80
#define BK3_81  0x81
#define BK3_82  0x82
#define BK3_83  0x83
#define BK3_84  0x84
#define BK3_85  0x85
#define BK3_86  0x86
// line buffer configuration
#define BK3_87  0x87
#define BK3_88  0x88
// De-interlace
// De-interlace control
#define BK3_89  0x89
#define BK3_8A  0x8A
#define BK3_8B  0x8B
#define BK3_8C  0x8C
#define BK3_8D  0x8D
#define BK3_8E  0x8E
#define BK3_8F  0x8F
#define BK3_90  0x90
#define BK3_91  0x91
#define BK3_92  0x92
#define BK3_93  0x93
#define BK3_94  0x94
#define BK3_95  0x95
// VBI sample range
#define BK3_96  0x96
#define BK3_97  0x97
#define BK3_98  0x98
#define BK3_99  0x99
#define BK3_9A  0x9A
#define BK3_9B  0x9B
#define BK3_9C  0x9C
#define BK3_9D  0x9D
#define BK3_9E  0x9E
#define BK3_9F  0x9F
//#ifdef PICASSO // ===>
// OSD-MIU interface
#define BK3_A0  0xA0
#define BK3_A1  0xA1
#define BK3_A2  0xA2
#define BK3_A3  0xA3
#define BK3_A4  0xA4
#define BK3_A5  0xA5
#define BK3_A7  0xA7
#define BK3_A8  0xA8
// OSD block clear register
#define BK3_A9  0xA9
#define BK3_AA  0xAA
#define BK3_AB  0xAB
#define BK3_AC  0xAC
#define BK3_AD  0xAD
#define BK3_AE  0xAE
#define BK3_AF  0xAF
#define BK3_B0  0xB0
//#endif // <===
// Luma curve and Histogram function control
#define BK3_B5  0xB5
#define BK3_B6  0xB6
#define BK3_B7  0xB7
#define BK3_B8  0xB8
#define BK3_B9  0xB9
#define BK3_BA  0xBA
#define BK3_BB  0xBB
#define BK3_BC  0xBC
#define BK3_BD  0xBD
#define BK3_BE  0xBE
#define BK3_BF  0xBF
// Memory clock synthesizer, interface
// memory PLL synthesizer interface
#define BK3_C0  0xC0
#define BK3_C1  0xC1
#define BK3_C2  0xC2
#define BK3_C3  0xC3
#define BK3_C4  0xC4
#define BK3_C5  0xC5
#define BK3_C6  0xC6
#define BK3_C7  0xC7
#define BK3_C8  0xC8
#define BK3_C9  0xC9
#define BK3_CA  0xCA
#define BK3_CB  0xCB
#define BK3_CC  0xCC
#define BK3_CD  0xCD
// second (Flesh Tone) color matrix coefficient
#define BK3_CE  0xCE // coefficient 11
#define BK3_CF  0xCF
#define BK3_D0  0xD0 // coefficient 12
#define BK3_D1  0xD1
#define BK3_D2  0xD2 // coefficient 13
#define BK3_D3  0xD3
#define BK3_D4  0xD4 // coefficient 21
#define BK3_D5  0xD5
#define BK3_D6  0xD6 // coefficient 22
#define BK3_D7  0xD7
#define BK3_D8  0xD8 // coefficient 23
#define BK3_D9  0xD9
#define BK3_DA  0xDA // coefficient 31
#define BK3_DB  0xDB
#define BK3_DC  0xDC // coefficient 32
#define BK3_DD  0xDD
#define BK3_DE  0xDE // coefficient 33
#define BK3_DF  0xDF
// Mode Detect Status
// [Mode detect status]
#define BK3_E0  0xE0 // input sync monitor
#define BK3_E1  0xE1
#define BK3_E2  0xE2 // input vertical total, count by HSync
#define BK3_E3  0xE3
#define BK3_E4  0xE4 // input horizontal period, count by reference clock
#define BK3_E5  0xE5
// Sync change tolerance
#define BK3_E6  0xE6 // Hsync tolerance
#define BK3_E7  0xE7 // VSync tolerance
// status override/interlace detect
#define BK3_E8  0xE8
#define BK3_E9  0xE9
// Misc. Control
// coast control
#define BK3_ED  0xED
// GPO control
#define BK3_EE  0xEE
#define BK3_EF  0xEF
#define BK3_F0  0xF0
#define BK3_F1  0xF1
// input signal control
#define BK3_F3  0xF3

#define BK4_01  0x01
#define BK4_02  0x02
#define BK4_03  0x03
#define BK4_04  0x04
#define BK4_05  0x05
#define BK4_06  0x06
#define BK4_07  0x07
#define BK4_08  0x08
#define BK4_09  0x09
#define BK4_0A  0x0A
#define BK4_0B  0x0B
#define BK4_0C  0x0C
#define BK4_0D  0x0D
#define BK4_0E  0x0E
#define BK4_0F  0x0F
#define BK4_10  0x10
#define BK4_11  0x11
#define BK4_12  0x12
#define BK4_13  0x13
#define BK4_14  0x14
#define BK4_15  0x15
#define BK4_16  0x16
#define BK4_17  0x17
#define BK4_18  0x18
#define BK4_19  0x19
#define BK4_1A  0x1A
#define BK4_1B  0x1B
#define BK4_1C  0x1C
#define BK4_1D  0x1D
#define BK4_1E  0x1E
#define BK4_1F  0x1F
#define BK4_20  0x20
#define BK4_21  0x21
#define BK4_22  0x22
#define BK4_23  0x23
#define BK4_24  0x24
#define BK4_25  0x25
#define BK4_26  0x26
#define BK4_27  0x27
#define BK4_28  0x28
#define BK4_29  0x29
#define BK4_2A  0x2A
#define BK4_2B  0x2B
#define BK4_2C  0x2C
#define BK4_2D  0x2D
#define BK4_2E  0x2E
#define BK4_2F  0x2F
#define BK4_30  0x30
#define BK4_31  0x31
#define BK4_32  0x32
#define BK4_33  0x33
#define BK4_34  0x34
#define BK4_35  0x35
#define BK4_3E  0x3E
#define BK4_3F  0x3F

#define BK4_40  0x40
#define BK4_41  0x41
#define BK4_42  0x42
#define BK4_43  0x43
#define BK4_44  0x44
#define BK4_45  0x45
#define BK4_46  0x46
#define BK4_47  0x47
#define BK4_48  0x48
#define BK4_49  0x49
#define BK4_4A  0x4A
#define BK4_4B  0x4B
#define BK4_4C  0x4C
#define BK4_4D  0x4D
#define BK4_4E  0x4E
#define BK4_4F  0x4F
#define BK4_50  0x50
#define BK4_51  0x51
#define BK4_52  0x52
#define BK4_53  0x53
// OSD MIU interface
#define BK4_60  0x60
#define BK4_61  0x61
#define BK4_62  0x62
#define BK4_63  0x63
#define BK4_64  0x64
#define BK4_65  0x65
#define BK4_66  0x66
#define BK4_67  0x67
// Audio Delay
#define BK4_68  0x68
#define BK4_69  0x69
#define BK4_6A  0x6A
#define BK4_6B  0x6B
#define BK4_6C  0x6C
#define BK4_6D  0x6D
#define BK4_6E  0x6E
#define BK4_6F  0x6F
// OSD clear function
#define BK4_70  0x70
#define BK4_71  0x71
#define BK4_72  0x72
#define BK4_73  0x73
#define BK4_74  0x74
#define BK4_75  0x75
#define BK4_76  0x76
#define BK4_77  0x77
#define BK4_78  0x78
#define BK4_79  0x79
#define BK4_7A  0x7A
#define BK4_7B  0x7B
#define BK4_7C  0x7C
#define BK4_7D  0x7D
#define BK4_7E  0x7E
#define BK4_7F  0x7F
// OSD write interface
#define BK4_80  0x80
#define BK4_81  0x81
#define BK4_82  0x82
#define BK4_83  0x83
#define BK4_84  0x84
#define BK4_85  0x85
#define BK4_86  0x86
#define BK4_87  0x87
#define BK4_88  0x88
#define BK4_89  0x89
// OSD display interface
#define BK4_8A  0x8A
#define BK4_8B  0x8B
#define BK4_8C  0x8C
#define BK4_8D  0x8D
#define BK4_8E  0x8E
#define BK4_8F  0x8F
#define BK4_90  0x90
#define BK4_91  0x91
#define BK4_92  0x92
#define BK4_93  0x93
#define BK4_94  0x94
#define BK4_95  0x95
#define BK4_96  0x96
#define BK4_97  0x97
#define BK4_98  0x98
#define BK4_99  0x99
#define BK4_9A  0x9A
#define BK4_9B  0x9B
// VBI for Main window
#define BK4_A0  0xA0
#define BK4_A1  0xA1
#define BK4_A2  0xA2
#define BK4_A3  0xA3
#define BK4_A4  0xA4
// VBI for Sub window
#define BK4_A5  0xA5
#define BK4_A6  0xA6
#define BK4_A7  0xA7
#define BK4_A9  0xA9
#define BK4_AA  0xAA
#define BK4_AB  0xAB
#define BK4_AC  0xAC
#define BK4_AD  0xAD
#define BK4_AE  0xAE
#define BK4_AF  0xAF
#define BK4_B0  0xB0
#define BK4_B1  0xB1
#define BK4_B2  0xB2
#define BK4_B3  0xB3
#define BK4_B4  0xB4
#define BK4_B5  0xB5
#define BK4_B6  0xB6
#define BK4_B7  0xB7
#define BK4_B8  0xB8
#define BK4_B9  0xB9
#define BK4_BA  0xBA
#define BK4_BB  0xBB
#define BK4_BC  0xBC
#define BK4_BD  0xBD
#define BK4_BE  0xBE
#define BK4_BF  0xBF
#define BK4_C0  0xC0

// Bank = 06
#define BK6_31  0x31
#define BK6_32  0x32
#define BK6_33  0x33
#define BK6_34  0x34
#define BK6_35  0x35
#define BK6_36  0x36
#define BK6_37  0x37
#define BK6_38  0x38
#define BK6_39  0x39

#ifdef TSUMXXT//#if (ChipID == ChipTSUMT)		//110311 Modify
///////////////////////////////////////////////////////
// #if ChipID==ChipTSUMT // Rose
// ADC_ATOP = Bank 0x08
// ADC_DTOP = Bank 0x07
// #enidf
///////////////////////////////////////////////////////
#ifndef REG_TABLE_END
#define REG_TABLE_END            0xFFFF
#endif

#define REGBANKADC_ATOP        0x08
#define REGBANKADC_DTOP        0x07

#define REG_ADC_ATOP_BASE        0x0800
#define REG_ADC_DTOP_BASE        0x0700

#define REG_ADC_ATOP_00_L        (REG_ADC_ATOP_BASE + 0x00)
#define REG_ADC_ATOP_00_H        (REG_ADC_ATOP_BASE + 0x01)
#define REG_ADC_ATOP_01_L        (REG_ADC_ATOP_BASE + 0x02)
#define REG_ADC_ATOP_01_H        (REG_ADC_ATOP_BASE + 0x03)
#define REG_ADC_ATOP_02_L        (REG_ADC_ATOP_BASE + 0x04)
#define REG_ADC_ATOP_02_H        (REG_ADC_ATOP_BASE + 0x05)
#define REG_ADC_ATOP_03_L        (REG_ADC_ATOP_BASE + 0x06)
#define REG_ADC_ATOP_03_H        (REG_ADC_ATOP_BASE + 0x07)
#define REG_ADC_ATOP_04_L        (REG_ADC_ATOP_BASE + 0x08)
#define REG_ADC_ATOP_04_H        (REG_ADC_ATOP_BASE + 0x09)
#define REG_ADC_ATOP_05_L        (REG_ADC_ATOP_BASE + 0x0A)
#define REG_ADC_ATOP_05_H        (REG_ADC_ATOP_BASE + 0x0B)
#define REG_ADC_ATOP_06_L        (REG_ADC_ATOP_BASE + 0x0C)
#define REG_ADC_ATOP_06_H        (REG_ADC_ATOP_BASE + 0x0D)
#define REG_ADC_ATOP_07_L        (REG_ADC_ATOP_BASE + 0x0E)
#define REG_ADC_ATOP_07_H        (REG_ADC_ATOP_BASE + 0x0F)
#define REG_ADC_ATOP_08_L        (REG_ADC_ATOP_BASE + 0x10)
#define REG_ADC_ATOP_08_H        (REG_ADC_ATOP_BASE + 0x11)
#define REG_ADC_ATOP_09_L        (REG_ADC_ATOP_BASE + 0x12)
#define REG_ADC_ATOP_09_H        (REG_ADC_ATOP_BASE + 0x13)
#define REG_ADC_ATOP_0A_L        (REG_ADC_ATOP_BASE + 0x14)
#define REG_ADC_ATOP_0A_H        (REG_ADC_ATOP_BASE + 0x15)
#define REG_ADC_ATOP_0B_L        (REG_ADC_ATOP_BASE + 0x16)
#define REG_ADC_ATOP_0B_H        (REG_ADC_ATOP_BASE + 0x17)
#define REG_ADC_ATOP_0C_L       (REG_ADC_ATOP_BASE + 0x18)
#define REG_ADC_ATOP_0C_H       (REG_ADC_ATOP_BASE + 0x19)
#define REG_ADC_ATOP_0D_L       (REG_ADC_ATOP_BASE + 0x1A)
#define REG_ADC_ATOP_0D_H       (REG_ADC_ATOP_BASE + 0x1B)
#define REG_ADC_ATOP_0E_L       (REG_ADC_ATOP_BASE + 0x1C)
#define REG_ADC_ATOP_0E_H       (REG_ADC_ATOP_BASE + 0x1D)
#define REG_ADC_ATOP_0F_L       (REG_ADC_ATOP_BASE + 0x1E)
#define REG_ADC_ATOP_0F_H       (REG_ADC_ATOP_BASE + 0x1F)
#define REG_ADC_ATOP_10_L       (REG_ADC_ATOP_BASE + 0x20)
#define REG_ADC_ATOP_10_H       (REG_ADC_ATOP_BASE + 0x21)
#define REG_ADC_ATOP_11_L       (REG_ADC_ATOP_BASE + 0x22)
#define REG_ADC_ATOP_11_H       (REG_ADC_ATOP_BASE + 0x23)
#define REG_ADC_ATOP_12_L       (REG_ADC_ATOP_BASE + 0x24)
#define REG_ADC_ATOP_12_H       (REG_ADC_ATOP_BASE + 0x25)
#define REG_ADC_ATOP_13_L       (REG_ADC_ATOP_BASE + 0x26)
#define REG_ADC_ATOP_13_H       (REG_ADC_ATOP_BASE + 0x27)
#define REG_ADC_ATOP_14_L       (REG_ADC_ATOP_BASE + 0x28)
#define REG_ADC_ATOP_14_H       (REG_ADC_ATOP_BASE + 0x29)
#define REG_ADC_ATOP_15_L       (REG_ADC_ATOP_BASE + 0x2A)
#define REG_ADC_ATOP_15_H       (REG_ADC_ATOP_BASE + 0x2B)
#define REG_ADC_ATOP_16_L       (REG_ADC_ATOP_BASE + 0x2C)
#define REG_ADC_ATOP_16_H       (REG_ADC_ATOP_BASE + 0x2D)
#define REG_ADC_ATOP_17_L       (REG_ADC_ATOP_BASE + 0x2E)
#define REG_ADC_ATOP_17_H       (REG_ADC_ATOP_BASE + 0x2F)
#define REG_ADC_ATOP_18_L       (REG_ADC_ATOP_BASE + 0x30)
#define REG_ADC_ATOP_18_H       (REG_ADC_ATOP_BASE + 0x31)
#define REG_ADC_ATOP_19_L       (REG_ADC_ATOP_BASE + 0x32)
#define REG_ADC_ATOP_19_H       (REG_ADC_ATOP_BASE + 0x33)
#define REG_ADC_ATOP_1A_L       (REG_ADC_ATOP_BASE + 0x34)
#define REG_ADC_ATOP_1A_H       (REG_ADC_ATOP_BASE + 0x35)
#define REG_ADC_ATOP_1B_L       (REG_ADC_ATOP_BASE + 0x36)
#define REG_ADC_ATOP_1B_H       (REG_ADC_ATOP_BASE + 0x37)
#define REG_ADC_ATOP_1C_L       (REG_ADC_ATOP_BASE + 0x38)
#define REG_ADC_ATOP_1C_H       (REG_ADC_ATOP_BASE + 0x39)
#define REG_ADC_ATOP_1D_L       (REG_ADC_ATOP_BASE + 0x3A)
#define REG_ADC_ATOP_1D_H       (REG_ADC_ATOP_BASE + 0x3B)
#define REG_ADC_ATOP_1E_L       (REG_ADC_ATOP_BASE + 0x3C)
#define REG_ADC_ATOP_1E_H       (REG_ADC_ATOP_BASE + 0x3D)
#define REG_ADC_ATOP_1F_L       (REG_ADC_ATOP_BASE + 0x3E)
#define REG_ADC_ATOP_1F_H       (REG_ADC_ATOP_BASE + 0x3F)
#define REG_ADC_ATOP_20_L       (REG_ADC_ATOP_BASE + 0x40)
#define REG_ADC_ATOP_20_H       (REG_ADC_ATOP_BASE + 0x41)
#define REG_ADC_ATOP_21_L       (REG_ADC_ATOP_BASE + 0x42)
#define REG_ADC_ATOP_21_H       (REG_ADC_ATOP_BASE + 0x43)
#define REG_ADC_ATOP_22_L       (REG_ADC_ATOP_BASE + 0x44)
#define REG_ADC_ATOP_22_H       (REG_ADC_ATOP_BASE + 0x45)
#define REG_ADC_ATOP_23_L       (REG_ADC_ATOP_BASE + 0x46)
#define REG_ADC_ATOP_23_H       (REG_ADC_ATOP_BASE + 0x47)
#define REG_ADC_ATOP_24_L       (REG_ADC_ATOP_BASE + 0x48)
#define REG_ADC_ATOP_24_H       (REG_ADC_ATOP_BASE + 0x49)
#define REG_ADC_ATOP_25_L       (REG_ADC_ATOP_BASE + 0x4A)
#define REG_ADC_ATOP_25_H       (REG_ADC_ATOP_BASE + 0x4B)
#define REG_ADC_ATOP_26_L       (REG_ADC_ATOP_BASE + 0x4C)
#define REG_ADC_ATOP_26_H       (REG_ADC_ATOP_BASE + 0x4D)
#define REG_ADC_ATOP_27_L       (REG_ADC_ATOP_BASE + 0x4E)
#define REG_ADC_ATOP_27_H       (REG_ADC_ATOP_BASE + 0x4F)
#define REG_ADC_ATOP_28_L       (REG_ADC_ATOP_BASE + 0x50)
#define REG_ADC_ATOP_28_H       (REG_ADC_ATOP_BASE + 0x51)
#define REG_ADC_ATOP_29_L       (REG_ADC_ATOP_BASE + 0x52)
#define REG_ADC_ATOP_29_H       (REG_ADC_ATOP_BASE + 0x53)
#define REG_ADC_ATOP_2A_L       (REG_ADC_ATOP_BASE + 0x54)
#define REG_ADC_ATOP_2A_H       (REG_ADC_ATOP_BASE + 0x55)
#define REG_ADC_ATOP_2B_L       (REG_ADC_ATOP_BASE + 0x56)
#define REG_ADC_ATOP_2B_H       (REG_ADC_ATOP_BASE + 0x57)
#define REG_ADC_ATOP_2C_L       (REG_ADC_ATOP_BASE + 0x58)
#define REG_ADC_ATOP_2C_H       (REG_ADC_ATOP_BASE + 0x59)
#define REG_ADC_ATOP_2D_L       (REG_ADC_ATOP_BASE + 0x5A)
#define REG_ADC_ATOP_2D_H       (REG_ADC_ATOP_BASE + 0x5B)
#define REG_ADC_ATOP_2E_L       (REG_ADC_ATOP_BASE + 0x5C)
#define REG_ADC_ATOP_2E_H       (REG_ADC_ATOP_BASE + 0x5D)
#define REG_ADC_ATOP_2F_L       (REG_ADC_ATOP_BASE + 0x5E)
#define REG_ADC_ATOP_2F_H       (REG_ADC_ATOP_BASE + 0x5F)
#define REG_ADC_ATOP_30_L       (REG_ADC_ATOP_BASE + 0x60)
#define REG_ADC_ATOP_30_H       (REG_ADC_ATOP_BASE + 0x61)
#define REG_ADC_ATOP_31_L       (REG_ADC_ATOP_BASE + 0x62)
#define REG_ADC_ATOP_31_H       (REG_ADC_ATOP_BASE + 0x63)
#define REG_ADC_ATOP_32_L       (REG_ADC_ATOP_BASE + 0x64)
#define REG_ADC_ATOP_32_H       (REG_ADC_ATOP_BASE + 0x65)
#define REG_ADC_ATOP_33_L       (REG_ADC_ATOP_BASE + 0x66)
#define REG_ADC_ATOP_33_H       (REG_ADC_ATOP_BASE + 0x67)
#define REG_ADC_ATOP_34_L       (REG_ADC_ATOP_BASE + 0x68)
#define REG_ADC_ATOP_34_H       (REG_ADC_ATOP_BASE + 0x69)
#define REG_ADC_ATOP_35_L       (REG_ADC_ATOP_BASE + 0x6A)
#define REG_ADC_ATOP_35_H       (REG_ADC_ATOP_BASE + 0x6B)
#define REG_ADC_ATOP_36_L       (REG_ADC_ATOP_BASE + 0x6C)
#define REG_ADC_ATOP_36_H       (REG_ADC_ATOP_BASE + 0x6D)
#define REG_ADC_ATOP_37_L       (REG_ADC_ATOP_BASE + 0x6E)
#define REG_ADC_ATOP_37_H       (REG_ADC_ATOP_BASE + 0x6F)
#define REG_ADC_ATOP_38_L       (REG_ADC_ATOP_BASE + 0x70)
#define REG_ADC_ATOP_38_H       (REG_ADC_ATOP_BASE + 0x71)
#define REG_ADC_ATOP_39_L       (REG_ADC_ATOP_BASE + 0x72)
#define REG_ADC_ATOP_39_H       (REG_ADC_ATOP_BASE + 0x73)
#define REG_ADC_ATOP_3A_L       (REG_ADC_ATOP_BASE + 0x74)
#define REG_ADC_ATOP_3A_H       (REG_ADC_ATOP_BASE + 0x75)
#define REG_ADC_ATOP_3B_L       (REG_ADC_ATOP_BASE + 0x76)
#define REG_ADC_ATOP_3B_H       (REG_ADC_ATOP_BASE + 0x77)
#define REG_ADC_ATOP_3C_L       (REG_ADC_ATOP_BASE + 0x78)
#define REG_ADC_ATOP_3C_H       (REG_ADC_ATOP_BASE + 0x79)
#define REG_ADC_ATOP_3D_L       (REG_ADC_ATOP_BASE + 0x7A)
#define REG_ADC_ATOP_3D_H       (REG_ADC_ATOP_BASE + 0x7B)
#define REG_ADC_ATOP_3E_L       (REG_ADC_ATOP_BASE + 0x7C)
#define REG_ADC_ATOP_3E_H       (REG_ADC_ATOP_BASE + 0x7D)
#define REG_ADC_ATOP_3F_L       (REG_ADC_ATOP_BASE + 0x7E)
#define REG_ADC_ATOP_3F_H       (REG_ADC_ATOP_BASE + 0x7F)
#define REG_ADC_ATOP_40_L       (REG_ADC_ATOP_BASE + 0x80)
#define REG_ADC_ATOP_40_H       (REG_ADC_ATOP_BASE + 0x81)
#define REG_ADC_ATOP_41_L       (REG_ADC_ATOP_BASE + 0x82)
#define REG_ADC_ATOP_41_H       (REG_ADC_ATOP_BASE + 0x83)
#define REG_ADC_ATOP_42_L       (REG_ADC_ATOP_BASE + 0x84)
#define REG_ADC_ATOP_42_H       (REG_ADC_ATOP_BASE + 0x85)
#define REG_ADC_ATOP_43_L       (REG_ADC_ATOP_BASE + 0x86)
#define REG_ADC_ATOP_43_H       (REG_ADC_ATOP_BASE + 0x87)
#define REG_ADC_ATOP_44_L       (REG_ADC_ATOP_BASE + 0x88)
#define REG_ADC_ATOP_44_H       (REG_ADC_ATOP_BASE + 0x89)
#define REG_ADC_ATOP_45_L       (REG_ADC_ATOP_BASE + 0x8A)
#define REG_ADC_ATOP_45_H       (REG_ADC_ATOP_BASE + 0x8B)
#define REG_ADC_ATOP_46_L       (REG_ADC_ATOP_BASE + 0x8C)
#define REG_ADC_ATOP_46_H       (REG_ADC_ATOP_BASE + 0x8D)
#define REG_ADC_ATOP_47_L       (REG_ADC_ATOP_BASE + 0x8E)
#define REG_ADC_ATOP_47_H       (REG_ADC_ATOP_BASE + 0x8F)
#define REG_ADC_ATOP_48_L       (REG_ADC_ATOP_BASE + 0x90)
#define REG_ADC_ATOP_48_H       (REG_ADC_ATOP_BASE + 0x91)
#define REG_ADC_ATOP_49_L       (REG_ADC_ATOP_BASE + 0x92)
#define REG_ADC_ATOP_49_H       (REG_ADC_ATOP_BASE + 0x93)
#define REG_ADC_ATOP_4A_L       (REG_ADC_ATOP_BASE + 0x94)
#define REG_ADC_ATOP_4A_H       (REG_ADC_ATOP_BASE + 0x95)
#define REG_ADC_ATOP_4B_L       (REG_ADC_ATOP_BASE + 0x96)
#define REG_ADC_ATOP_4B_H       (REG_ADC_ATOP_BASE + 0x97)
#define REG_ADC_ATOP_4C_L       (REG_ADC_ATOP_BASE + 0x98)
#define REG_ADC_ATOP_4C_H       (REG_ADC_ATOP_BASE + 0x99)
#define REG_ADC_ATOP_4D_L       (REG_ADC_ATOP_BASE + 0x9A)
#define REG_ADC_ATOP_4D_H       (REG_ADC_ATOP_BASE + 0x9B)
#define REG_ADC_ATOP_4E_L       (REG_ADC_ATOP_BASE + 0x9C)
#define REG_ADC_ATOP_4E_H       (REG_ADC_ATOP_BASE + 0x9D)
#define REG_ADC_ATOP_4F_L       (REG_ADC_ATOP_BASE + 0x9E)
#define REG_ADC_ATOP_4F_H       (REG_ADC_ATOP_BASE + 0x9F)
#define REG_ADC_ATOP_50_L       (REG_ADC_ATOP_BASE + 0xA0)
#define REG_ADC_ATOP_50_H       (REG_ADC_ATOP_BASE + 0xA1)
#define REG_ADC_ATOP_51_L       (REG_ADC_ATOP_BASE + 0xA2)
#define REG_ADC_ATOP_51_H       (REG_ADC_ATOP_BASE + 0xA3)
#define REG_ADC_ATOP_52_L       (REG_ADC_ATOP_BASE + 0xA4)
#define REG_ADC_ATOP_52_H       (REG_ADC_ATOP_BASE + 0xA5)
#define REG_ADC_ATOP_53_L       (REG_ADC_ATOP_BASE + 0xA6)
#define REG_ADC_ATOP_53_H       (REG_ADC_ATOP_BASE + 0xA7)
#define REG_ADC_ATOP_54_L       (REG_ADC_ATOP_BASE + 0xA8)
#define REG_ADC_ATOP_54_H       (REG_ADC_ATOP_BASE + 0xA9)
#define REG_ADC_ATOP_55_L       (REG_ADC_ATOP_BASE + 0xAA)
#define REG_ADC_ATOP_55_H       (REG_ADC_ATOP_BASE + 0xAB)
#define REG_ADC_ATOP_56_L       (REG_ADC_ATOP_BASE + 0xAC)
#define REG_ADC_ATOP_56_H       (REG_ADC_ATOP_BASE + 0xAD)
#define REG_ADC_ATOP_57_L       (REG_ADC_ATOP_BASE + 0xAE)
#define REG_ADC_ATOP_57_H       (REG_ADC_ATOP_BASE + 0xAF)
#define REG_ADC_ATOP_58_L       (REG_ADC_ATOP_BASE + 0xB0)
#define REG_ADC_ATOP_58_H       (REG_ADC_ATOP_BASE + 0xB1)
#define REG_ADC_ATOP_59_L       (REG_ADC_ATOP_BASE + 0xB2)
#define REG_ADC_ATOP_59_H       (REG_ADC_ATOP_BASE + 0xB3)
#define REG_ADC_ATOP_5A_L       (REG_ADC_ATOP_BASE + 0xB4)
#define REG_ADC_ATOP_5A_H       (REG_ADC_ATOP_BASE + 0xB5)
#define REG_ADC_ATOP_5B_L       (REG_ADC_ATOP_BASE + 0xB6)
#define REG_ADC_ATOP_5B_H       (REG_ADC_ATOP_BASE + 0xB7)
#define REG_ADC_ATOP_5C_L       (REG_ADC_ATOP_BASE + 0xB8)
#define REG_ADC_ATOP_5C_H       (REG_ADC_ATOP_BASE + 0xB9)
#define REG_ADC_ATOP_5D_L       (REG_ADC_ATOP_BASE + 0xBA)
#define REG_ADC_ATOP_5D_H       (REG_ADC_ATOP_BASE + 0xBB)
#define REG_ADC_ATOP_5E_L       (REG_ADC_ATOP_BASE + 0xBC)
#define REG_ADC_ATOP_5E_H       (REG_ADC_ATOP_BASE + 0xBD)
#define REG_ADC_ATOP_5F_L       (REG_ADC_ATOP_BASE + 0xBE)
#define REG_ADC_ATOP_5F_H       (REG_ADC_ATOP_BASE + 0xBF)
#define REG_ADC_ATOP_60_L       (REG_ADC_ATOP_BASE + 0xC0)
#define REG_ADC_ATOP_60_H       (REG_ADC_ATOP_BASE + 0xC1)
#define REG_ADC_ATOP_61_L       (REG_ADC_ATOP_BASE + 0xC2)
#define REG_ADC_ATOP_61_H       (REG_ADC_ATOP_BASE + 0xC3)
#define REG_ADC_ATOP_62_L       (REG_ADC_ATOP_BASE + 0xC4)
#define REG_ADC_ATOP_62_H       (REG_ADC_ATOP_BASE + 0xC5)
#define REG_ADC_ATOP_63_L       (REG_ADC_ATOP_BASE + 0xC6)
#define REG_ADC_ATOP_63_H       (REG_ADC_ATOP_BASE + 0xC7)
#define REG_ADC_ATOP_64_L       (REG_ADC_ATOP_BASE + 0xC8)
#define REG_ADC_ATOP_64_H       (REG_ADC_ATOP_BASE + 0xC9)
#define REG_ADC_ATOP_65_L       (REG_ADC_ATOP_BASE + 0xCA)
#define REG_ADC_ATOP_65_H       (REG_ADC_ATOP_BASE + 0xCB)
#define REG_ADC_ATOP_66_L       (REG_ADC_ATOP_BASE + 0xCC)
#define REG_ADC_ATOP_66_H       (REG_ADC_ATOP_BASE + 0xCD)
#define REG_ADC_ATOP_67_L       (REG_ADC_ATOP_BASE + 0xCE)
#define REG_ADC_ATOP_67_H       (REG_ADC_ATOP_BASE + 0xCF)
#define REG_ADC_ATOP_68_L       (REG_ADC_ATOP_BASE + 0xD0)
#define REG_ADC_ATOP_68_H       (REG_ADC_ATOP_BASE + 0xD1)
#define REG_ADC_ATOP_69_L       (REG_ADC_ATOP_BASE + 0xD2)
#define REG_ADC_ATOP_69_H       (REG_ADC_ATOP_BASE + 0xD3)
#define REG_ADC_ATOP_6A_L       (REG_ADC_ATOP_BASE + 0xD4)
#define REG_ADC_ATOP_6A_H       (REG_ADC_ATOP_BASE + 0xD5)
#define REG_ADC_ATOP_6B_L       (REG_ADC_ATOP_BASE + 0xD6)
#define REG_ADC_ATOP_6B_H       (REG_ADC_ATOP_BASE + 0xD7)
#define REG_ADC_ATOP_6C_L       (REG_ADC_ATOP_BASE + 0xD8)
#define REG_ADC_ATOP_6C_H       (REG_ADC_ATOP_BASE + 0xD9)
#define REG_ADC_ATOP_6D_L       (REG_ADC_ATOP_BASE + 0xDA)
#define REG_ADC_ATOP_6D_H       (REG_ADC_ATOP_BASE + 0xDB)
#define REG_ADC_ATOP_6E_L       (REG_ADC_ATOP_BASE + 0xDC)
#define REG_ADC_ATOP_6E_H       (REG_ADC_ATOP_BASE + 0xDD)
#define REG_ADC_ATOP_6F_L       (REG_ADC_ATOP_BASE + 0xDE)
#define REG_ADC_ATOP_6F_H       (REG_ADC_ATOP_BASE + 0xDF)
#define REG_ADC_ATOP_70_L       (REG_ADC_ATOP_BASE + 0xE0)
#define REG_ADC_ATOP_70_H       (REG_ADC_ATOP_BASE + 0xE1)
#define REG_ADC_ATOP_71_L       (REG_ADC_ATOP_BASE + 0xE2)
#define REG_ADC_ATOP_71_H       (REG_ADC_ATOP_BASE + 0xE3)
#define REG_ADC_ATOP_72_L       (REG_ADC_ATOP_BASE + 0xE4)
#define REG_ADC_ATOP_72_H       (REG_ADC_ATOP_BASE + 0xE5)
#define REG_ADC_ATOP_73_L       (REG_ADC_ATOP_BASE + 0xE6)
#define REG_ADC_ATOP_73_H       (REG_ADC_ATOP_BASE + 0xE7)
#define REG_ADC_ATOP_74_L       (REG_ADC_ATOP_BASE + 0xE8)
#define REG_ADC_ATOP_74_H       (REG_ADC_ATOP_BASE + 0xE9)
#define REG_ADC_ATOP_75_L       (REG_ADC_ATOP_BASE + 0xEA)
#define REG_ADC_ATOP_75_H       (REG_ADC_ATOP_BASE + 0xEB)
#define REG_ADC_ATOP_76_L       (REG_ADC_ATOP_BASE + 0xEC)
#define REG_ADC_ATOP_76_H       (REG_ADC_ATOP_BASE + 0xED)
#define REG_ADC_ATOP_77_L       (REG_ADC_ATOP_BASE + 0xEE)
#define REG_ADC_ATOP_77_H       (REG_ADC_ATOP_BASE + 0xEF)
#define REG_ADC_ATOP_78_L       (REG_ADC_ATOP_BASE + 0xF0)
#define REG_ADC_ATOP_78_H       (REG_ADC_ATOP_BASE + 0xF1)
#define REG_ADC_ATOP_79_L       (REG_ADC_ATOP_BASE + 0xF2)
#define REG_ADC_ATOP_79_H       (REG_ADC_ATOP_BASE + 0xF3)
#define REG_ADC_ATOP_7A_L       (REG_ADC_ATOP_BASE + 0xF4)
#define REG_ADC_ATOP_7A_H       (REG_ADC_ATOP_BASE + 0xF5)
#define REG_ADC_ATOP_7B_L       (REG_ADC_ATOP_BASE + 0xF6)
#define REG_ADC_ATOP_7B_H       (REG_ADC_ATOP_BASE + 0xF7)
#define REG_ADC_ATOP_7C_L       (REG_ADC_ATOP_BASE + 0xF8)
#define REG_ADC_ATOP_7C_H       (REG_ADC_ATOP_BASE + 0xF9)
#define REG_ADC_ATOP_7D_L       (REG_ADC_ATOP_BASE + 0xFA)
#define REG_ADC_ATOP_7D_H       (REG_ADC_ATOP_BASE + 0xFB)
#define REG_ADC_ATOP_7E_L       (REG_ADC_ATOP_BASE + 0xFC)
#define REG_ADC_ATOP_7E_H       (REG_ADC_ATOP_BASE + 0xFD)
#define REG_ADC_ATOP_7F_L       (REG_ADC_ATOP_BASE + 0xFE)
#define REG_ADC_ATOP_7F_H       (REG_ADC_ATOP_BASE + 0xFF)

#define REG_ADC_DTOP_00_L        (REG_ADC_DTOP_BASE + 0x00)
#define REG_ADC_DTOP_00_H        (REG_ADC_DTOP_BASE + 0x01)
#define REG_ADC_DTOP_01_L        (REG_ADC_DTOP_BASE + 0x02)
#define REG_ADC_DTOP_01_H        (REG_ADC_DTOP_BASE + 0x03)
#define REG_ADC_DTOP_02_L        (REG_ADC_DTOP_BASE + 0x04)
#define REG_ADC_DTOP_02_H        (REG_ADC_DTOP_BASE + 0x05)
#define REG_ADC_DTOP_03_L        (REG_ADC_DTOP_BASE + 0x06)
#define REG_ADC_DTOP_03_H        (REG_ADC_DTOP_BASE + 0x07)
#define REG_ADC_DTOP_04_L        (REG_ADC_DTOP_BASE + 0x08)
#define REG_ADC_DTOP_04_H        (REG_ADC_DTOP_BASE + 0x09)
#define REG_ADC_DTOP_05_L        (REG_ADC_DTOP_BASE + 0x0A)
#define REG_ADC_DTOP_05_H        (REG_ADC_DTOP_BASE + 0x0B)
#define REG_ADC_DTOP_06_L        (REG_ADC_DTOP_BASE + 0x0C)
#define REG_ADC_DTOP_06_H        (REG_ADC_DTOP_BASE + 0x0D)
#define REG_ADC_DTOP_07_L        (REG_ADC_DTOP_BASE + 0x0E)
#define REG_ADC_DTOP_07_H        (REG_ADC_DTOP_BASE + 0x0F)
#define REG_ADC_DTOP_08_L        (REG_ADC_DTOP_BASE + 0x10)
#define REG_ADC_DTOP_08_H        (REG_ADC_DTOP_BASE + 0x11)
#define REG_ADC_DTOP_09_L        (REG_ADC_DTOP_BASE + 0x12)
#define REG_ADC_DTOP_09_H        (REG_ADC_DTOP_BASE + 0x13)
#define REG_ADC_DTOP_0A_L        (REG_ADC_DTOP_BASE + 0x14)
#define REG_ADC_DTOP_0A_H        (REG_ADC_DTOP_BASE + 0x15)
#define REG_ADC_DTOP_0B_L        (REG_ADC_DTOP_BASE + 0x16)
#define REG_ADC_DTOP_0B_H        (REG_ADC_DTOP_BASE + 0x17)
#define REG_ADC_DTOP_0C_L       (REG_ADC_DTOP_BASE + 0x18)
#define REG_ADC_DTOP_0C_H       (REG_ADC_DTOP_BASE + 0x19)
#define REG_ADC_DTOP_0D_L       (REG_ADC_DTOP_BASE + 0x1A)
#define REG_ADC_DTOP_0D_H       (REG_ADC_DTOP_BASE + 0x1B)
#define REG_ADC_DTOP_0E_L       (REG_ADC_DTOP_BASE + 0x1C)
#define REG_ADC_DTOP_0E_H       (REG_ADC_DTOP_BASE + 0x1D)
#define REG_ADC_DTOP_0F_L       (REG_ADC_DTOP_BASE + 0x1E)
#define REG_ADC_DTOP_0F_H       (REG_ADC_DTOP_BASE + 0x1F)
#define REG_ADC_DTOP_10_L       (REG_ADC_DTOP_BASE + 0x20)
#define REG_ADC_DTOP_10_H       (REG_ADC_DTOP_BASE + 0x21)
#define REG_ADC_DTOP_11_L       (REG_ADC_DTOP_BASE + 0x22)
#define REG_ADC_DTOP_11_H       (REG_ADC_DTOP_BASE + 0x23)
#define REG_ADC_DTOP_12_L       (REG_ADC_DTOP_BASE + 0x24)
#define REG_ADC_DTOP_12_H       (REG_ADC_DTOP_BASE + 0x25)
#define REG_ADC_DTOP_13_L       (REG_ADC_DTOP_BASE + 0x26)
#define REG_ADC_DTOP_13_H       (REG_ADC_DTOP_BASE + 0x27)
#define REG_ADC_DTOP_14_L       (REG_ADC_DTOP_BASE + 0x28)
#define REG_ADC_DTOP_14_H       (REG_ADC_DTOP_BASE + 0x29)
#define REG_ADC_DTOP_15_L       (REG_ADC_DTOP_BASE + 0x2A)
#define REG_ADC_DTOP_15_H       (REG_ADC_DTOP_BASE + 0x2B)
#define REG_ADC_DTOP_16_L       (REG_ADC_DTOP_BASE + 0x2C)
#define REG_ADC_DTOP_16_H       (REG_ADC_DTOP_BASE + 0x2D)
#define REG_ADC_DTOP_17_L       (REG_ADC_DTOP_BASE + 0x2E)
#define REG_ADC_DTOP_17_H       (REG_ADC_DTOP_BASE + 0x2F)
#define REG_ADC_DTOP_18_L       (REG_ADC_DTOP_BASE + 0x30)
#define REG_ADC_DTOP_18_H       (REG_ADC_DTOP_BASE + 0x31)
#define REG_ADC_DTOP_19_L       (REG_ADC_DTOP_BASE + 0x32)
#define REG_ADC_DTOP_19_H       (REG_ADC_DTOP_BASE + 0x33)
#define REG_ADC_DTOP_1A_L       (REG_ADC_DTOP_BASE + 0x34)
#define REG_ADC_DTOP_1A_H       (REG_ADC_DTOP_BASE + 0x35)
#define REG_ADC_DTOP_1B_L       (REG_ADC_DTOP_BASE + 0x36)
#define REG_ADC_DTOP_1B_H       (REG_ADC_DTOP_BASE + 0x37)
#define REG_ADC_DTOP_1C_L       (REG_ADC_DTOP_BASE + 0x38)
#define REG_ADC_DTOP_1C_H       (REG_ADC_DTOP_BASE + 0x39)
#define REG_ADC_DTOP_1D_L       (REG_ADC_DTOP_BASE + 0x3A)
#define REG_ADC_DTOP_1D_H       (REG_ADC_DTOP_BASE + 0x3B)
#define REG_ADC_DTOP_1E_L       (REG_ADC_DTOP_BASE + 0x3C)
#define REG_ADC_DTOP_1E_H       (REG_ADC_DTOP_BASE + 0x3D)
#define REG_ADC_DTOP_1F_L       (REG_ADC_DTOP_BASE + 0x3E)
#define REG_ADC_DTOP_1F_H       (REG_ADC_DTOP_BASE + 0x3F)
#define REG_ADC_DTOP_20_L       (REG_ADC_DTOP_BASE + 0x40)
#define REG_ADC_DTOP_20_H       (REG_ADC_DTOP_BASE + 0x41)
#define REG_ADC_DTOP_21_L       (REG_ADC_DTOP_BASE + 0x42)
#define REG_ADC_DTOP_21_H       (REG_ADC_DTOP_BASE + 0x43)
#define REG_ADC_DTOP_22_L       (REG_ADC_DTOP_BASE + 0x44)
#define REG_ADC_DTOP_22_H       (REG_ADC_DTOP_BASE + 0x45)
#define REG_ADC_DTOP_23_L       (REG_ADC_DTOP_BASE + 0x46)
#define REG_ADC_DTOP_23_H       (REG_ADC_DTOP_BASE + 0x47)
#define REG_ADC_DTOP_24_L       (REG_ADC_DTOP_BASE + 0x48)
#define REG_ADC_DTOP_24_H       (REG_ADC_DTOP_BASE + 0x49)
#define REG_ADC_DTOP_25_L       (REG_ADC_DTOP_BASE + 0x4A)
#define REG_ADC_DTOP_25_H       (REG_ADC_DTOP_BASE + 0x4B)
#define REG_ADC_DTOP_26_L       (REG_ADC_DTOP_BASE + 0x4C)
#define REG_ADC_DTOP_26_H       (REG_ADC_DTOP_BASE + 0x4D)
#define REG_ADC_DTOP_27_L       (REG_ADC_DTOP_BASE + 0x4E)
#define REG_ADC_DTOP_27_H       (REG_ADC_DTOP_BASE + 0x4F)
#define REG_ADC_DTOP_28_L       (REG_ADC_DTOP_BASE + 0x50)
#define REG_ADC_DTOP_28_H       (REG_ADC_DTOP_BASE + 0x51)
#define REG_ADC_DTOP_29_L       (REG_ADC_DTOP_BASE + 0x52)
#define REG_ADC_DTOP_29_H       (REG_ADC_DTOP_BASE + 0x53)
#define REG_ADC_DTOP_2A_L       (REG_ADC_DTOP_BASE + 0x54)
#define REG_ADC_DTOP_2A_H       (REG_ADC_DTOP_BASE + 0x55)
#define REG_ADC_DTOP_2B_L       (REG_ADC_DTOP_BASE + 0x56)
#define REG_ADC_DTOP_2B_H       (REG_ADC_DTOP_BASE + 0x57)
#define REG_ADC_DTOP_2C_L       (REG_ADC_DTOP_BASE + 0x58)
#define REG_ADC_DTOP_2C_H       (REG_ADC_DTOP_BASE + 0x59)
#define REG_ADC_DTOP_2D_L       (REG_ADC_DTOP_BASE + 0x5A)
#define REG_ADC_DTOP_2D_H       (REG_ADC_DTOP_BASE + 0x5B)
#define REG_ADC_DTOP_2E_L       (REG_ADC_DTOP_BASE + 0x5C)
#define REG_ADC_DTOP_2E_H       (REG_ADC_DTOP_BASE + 0x5D)
#define REG_ADC_DTOP_2F_L       (REG_ADC_DTOP_BASE + 0x5E)
#define REG_ADC_DTOP_2F_H       (REG_ADC_DTOP_BASE + 0x5F)
#define REG_ADC_DTOP_30_L       (REG_ADC_DTOP_BASE + 0x60)
#define REG_ADC_DTOP_30_H       (REG_ADC_DTOP_BASE + 0x61)
#define REG_ADC_DTOP_31_L       (REG_ADC_DTOP_BASE + 0x62)
#define REG_ADC_DTOP_31_H       (REG_ADC_DTOP_BASE + 0x63)
#define REG_ADC_DTOP_32_L       (REG_ADC_DTOP_BASE + 0x64)
#define REG_ADC_DTOP_32_H       (REG_ADC_DTOP_BASE + 0x65)
#define REG_ADC_DTOP_33_L       (REG_ADC_DTOP_BASE + 0x66)
#define REG_ADC_DTOP_33_H       (REG_ADC_DTOP_BASE + 0x67)
#define REG_ADC_DTOP_34_L       (REG_ADC_DTOP_BASE + 0x68)
#define REG_ADC_DTOP_34_H       (REG_ADC_DTOP_BASE + 0x69)
#define REG_ADC_DTOP_35_L       (REG_ADC_DTOP_BASE + 0x6A)
#define REG_ADC_DTOP_35_H       (REG_ADC_DTOP_BASE + 0x6B)
#define REG_ADC_DTOP_36_L       (REG_ADC_DTOP_BASE + 0x6C)
#define REG_ADC_DTOP_36_H       (REG_ADC_DTOP_BASE + 0x6D)
#define REG_ADC_DTOP_37_L       (REG_ADC_DTOP_BASE + 0x6E)
#define REG_ADC_DTOP_37_H       (REG_ADC_DTOP_BASE + 0x6F)
#define REG_ADC_DTOP_38_L       (REG_ADC_DTOP_BASE + 0x70)
#define REG_ADC_DTOP_38_H       (REG_ADC_DTOP_BASE + 0x71)
#define REG_ADC_DTOP_39_L       (REG_ADC_DTOP_BASE + 0x72)
#define REG_ADC_DTOP_39_H       (REG_ADC_DTOP_BASE + 0x73)
#define REG_ADC_DTOP_3A_L       (REG_ADC_DTOP_BASE + 0x74)
#define REG_ADC_DTOP_3A_H       (REG_ADC_DTOP_BASE + 0x75)
#define REG_ADC_DTOP_3B_L       (REG_ADC_DTOP_BASE + 0x76)
#define REG_ADC_DTOP_3B_H       (REG_ADC_DTOP_BASE + 0x77)
#define REG_ADC_DTOP_3C_L       (REG_ADC_DTOP_BASE + 0x78)
#define REG_ADC_DTOP_3C_H       (REG_ADC_DTOP_BASE + 0x79)
#define REG_ADC_DTOP_3D_L       (REG_ADC_DTOP_BASE + 0x7A)
#define REG_ADC_DTOP_3D_H       (REG_ADC_DTOP_BASE + 0x7B)
#define REG_ADC_DTOP_3E_L       (REG_ADC_DTOP_BASE + 0x7C)
#define REG_ADC_DTOP_3E_H       (REG_ADC_DTOP_BASE + 0x7D)
#define REG_ADC_DTOP_3F_L       (REG_ADC_DTOP_BASE + 0x7E)
#define REG_ADC_DTOP_3F_H       (REG_ADC_DTOP_BASE + 0x7F)
#define REG_ADC_DTOP_40_L       (REG_ADC_DTOP_BASE + 0x80)
#define REG_ADC_DTOP_40_H       (REG_ADC_DTOP_BASE + 0x81)
#define REG_ADC_DTOP_41_L       (REG_ADC_DTOP_BASE + 0x82)
#define REG_ADC_DTOP_41_H       (REG_ADC_DTOP_BASE + 0x83)
#define REG_ADC_DTOP_42_L       (REG_ADC_DTOP_BASE + 0x84)
#define REG_ADC_DTOP_42_H       (REG_ADC_DTOP_BASE + 0x85)
#define REG_ADC_DTOP_43_L       (REG_ADC_DTOP_BASE + 0x86)
#define REG_ADC_DTOP_43_H       (REG_ADC_DTOP_BASE + 0x87)
#define REG_ADC_DTOP_44_L       (REG_ADC_DTOP_BASE + 0x88)
#define REG_ADC_DTOP_44_H       (REG_ADC_DTOP_BASE + 0x89)
#define REG_ADC_DTOP_45_L       (REG_ADC_DTOP_BASE + 0x8A)
#define REG_ADC_DTOP_45_H       (REG_ADC_DTOP_BASE + 0x8B)
#define REG_ADC_DTOP_46_L       (REG_ADC_DTOP_BASE + 0x8C)
#define REG_ADC_DTOP_46_H       (REG_ADC_DTOP_BASE + 0x8D)
#define REG_ADC_DTOP_47_L       (REG_ADC_DTOP_BASE + 0x8E)
#define REG_ADC_DTOP_47_H       (REG_ADC_DTOP_BASE + 0x8F)
#define REG_ADC_DTOP_48_L       (REG_ADC_DTOP_BASE + 0x90)
#define REG_ADC_DTOP_48_H       (REG_ADC_DTOP_BASE + 0x91)
#define REG_ADC_DTOP_49_L       (REG_ADC_DTOP_BASE + 0x92)
#define REG_ADC_DTOP_49_H       (REG_ADC_DTOP_BASE + 0x93)
#define REG_ADC_DTOP_4A_L       (REG_ADC_DTOP_BASE + 0x94)
#define REG_ADC_DTOP_4A_H       (REG_ADC_DTOP_BASE + 0x95)
#define REG_ADC_DTOP_4B_L       (REG_ADC_DTOP_BASE + 0x96)
#define REG_ADC_DTOP_4B_H       (REG_ADC_DTOP_BASE + 0x97)
#define REG_ADC_DTOP_4C_L       (REG_ADC_DTOP_BASE + 0x98)
#define REG_ADC_DTOP_4C_H       (REG_ADC_DTOP_BASE + 0x99)
#define REG_ADC_DTOP_4D_L       (REG_ADC_DTOP_BASE + 0x9A)
#define REG_ADC_DTOP_4D_H       (REG_ADC_DTOP_BASE + 0x9B)
#define REG_ADC_DTOP_4E_L       (REG_ADC_DTOP_BASE + 0x9C)
#define REG_ADC_DTOP_4E_H       (REG_ADC_DTOP_BASE + 0x9D)
#define REG_ADC_DTOP_4F_L       (REG_ADC_DTOP_BASE + 0x9E)
#define REG_ADC_DTOP_4F_H       (REG_ADC_DTOP_BASE + 0x9F)
#define REG_ADC_DTOP_50_L       (REG_ADC_DTOP_BASE + 0xA0)
#define REG_ADC_DTOP_50_H       (REG_ADC_DTOP_BASE + 0xA1)
#define REG_ADC_DTOP_51_L       (REG_ADC_DTOP_BASE + 0xA2)
#define REG_ADC_DTOP_51_H       (REG_ADC_DTOP_BASE + 0xA3)
#define REG_ADC_DTOP_52_L       (REG_ADC_DTOP_BASE + 0xA4)
#define REG_ADC_DTOP_52_H       (REG_ADC_DTOP_BASE + 0xA5)
#define REG_ADC_DTOP_53_L       (REG_ADC_DTOP_BASE + 0xA6)
#define REG_ADC_DTOP_53_H       (REG_ADC_DTOP_BASE + 0xA7)
#define REG_ADC_DTOP_54_L       (REG_ADC_DTOP_BASE + 0xA8)
#define REG_ADC_DTOP_54_H       (REG_ADC_DTOP_BASE + 0xA9)
#define REG_ADC_DTOP_55_L       (REG_ADC_DTOP_BASE + 0xAA)
#define REG_ADC_DTOP_55_H       (REG_ADC_DTOP_BASE + 0xAB)
#define REG_ADC_DTOP_56_L       (REG_ADC_DTOP_BASE + 0xAC)
#define REG_ADC_DTOP_56_H       (REG_ADC_DTOP_BASE + 0xAD)
#define REG_ADC_DTOP_57_L       (REG_ADC_DTOP_BASE + 0xAE)
#define REG_ADC_DTOP_57_H       (REG_ADC_DTOP_BASE + 0xAF)
#define REG_ADC_DTOP_58_L       (REG_ADC_DTOP_BASE + 0xB0)
#define REG_ADC_DTOP_58_H       (REG_ADC_DTOP_BASE + 0xB1)
#define REG_ADC_DTOP_59_L       (REG_ADC_DTOP_BASE + 0xB2)
#define REG_ADC_DTOP_59_H       (REG_ADC_DTOP_BASE + 0xB3)
#define REG_ADC_DTOP_5A_L       (REG_ADC_DTOP_BASE + 0xB4)
#define REG_ADC_DTOP_5A_H       (REG_ADC_DTOP_BASE + 0xB5)
#define REG_ADC_DTOP_5B_L       (REG_ADC_DTOP_BASE + 0xB6)
#define REG_ADC_DTOP_5B_H       (REG_ADC_DTOP_BASE + 0xB7)
#define REG_ADC_DTOP_5C_L       (REG_ADC_DTOP_BASE + 0xB8)
#define REG_ADC_DTOP_5C_H       (REG_ADC_DTOP_BASE + 0xB9)
#define REG_ADC_DTOP_5D_L       (REG_ADC_DTOP_BASE + 0xBA)
#define REG_ADC_DTOP_5D_H       (REG_ADC_DTOP_BASE + 0xBB)
#define REG_ADC_DTOP_5E_L       (REG_ADC_DTOP_BASE + 0xBC)
#define REG_ADC_DTOP_5E_H       (REG_ADC_DTOP_BASE + 0xBD)
#define REG_ADC_DTOP_5F_L       (REG_ADC_DTOP_BASE + 0xBE)
#define REG_ADC_DTOP_5F_H       (REG_ADC_DTOP_BASE + 0xBF)
#define REG_ADC_DTOP_60_L       (REG_ADC_DTOP_BASE + 0xC0)
#define REG_ADC_DTOP_60_H       (REG_ADC_DTOP_BASE + 0xC1)
#define REG_ADC_DTOP_61_L       (REG_ADC_DTOP_BASE + 0xC2)
#define REG_ADC_DTOP_61_H       (REG_ADC_DTOP_BASE + 0xC3)
#define REG_ADC_DTOP_62_L       (REG_ADC_DTOP_BASE + 0xC4)
#define REG_ADC_DTOP_62_H       (REG_ADC_DTOP_BASE + 0xC5)
#define REG_ADC_DTOP_63_L       (REG_ADC_DTOP_BASE + 0xC6)
#define REG_ADC_DTOP_63_H       (REG_ADC_DTOP_BASE + 0xC7)
#define REG_ADC_DTOP_64_L       (REG_ADC_DTOP_BASE + 0xC8)
#define REG_ADC_DTOP_64_H       (REG_ADC_DTOP_BASE + 0xC9)
#define REG_ADC_DTOP_65_L       (REG_ADC_DTOP_BASE + 0xCA)
#define REG_ADC_DTOP_65_H       (REG_ADC_DTOP_BASE + 0xCB)
#define REG_ADC_DTOP_66_L       (REG_ADC_DTOP_BASE + 0xCC)
#define REG_ADC_DTOP_66_H       (REG_ADC_DTOP_BASE + 0xCD)
#define REG_ADC_DTOP_67_L       (REG_ADC_DTOP_BASE + 0xCE)
#define REG_ADC_DTOP_67_H       (REG_ADC_DTOP_BASE + 0xCF)
#define REG_ADC_DTOP_68_L       (REG_ADC_DTOP_BASE + 0xD0)
#define REG_ADC_DTOP_68_H       (REG_ADC_DTOP_BASE + 0xD1)
#define REG_ADC_DTOP_69_L       (REG_ADC_DTOP_BASE + 0xD2)
#define REG_ADC_DTOP_69_H       (REG_ADC_DTOP_BASE + 0xD3)
#define REG_ADC_DTOP_6A_L       (REG_ADC_DTOP_BASE + 0xD4)
#define REG_ADC_DTOP_6A_H       (REG_ADC_DTOP_BASE + 0xD5)
#define REG_ADC_DTOP_6B_L       (REG_ADC_DTOP_BASE + 0xD6)
#define REG_ADC_DTOP_6B_H       (REG_ADC_DTOP_BASE + 0xD7)
#define REG_ADC_DTOP_6C_L       (REG_ADC_DTOP_BASE + 0xD8)
#define REG_ADC_DTOP_6C_H       (REG_ADC_DTOP_BASE + 0xD9)
#define REG_ADC_DTOP_6D_L       (REG_ADC_DTOP_BASE + 0xDA)
#define REG_ADC_DTOP_6D_H       (REG_ADC_DTOP_BASE + 0xDB)
#define REG_ADC_DTOP_6E_L       (REG_ADC_DTOP_BASE + 0xDC)
#define REG_ADC_DTOP_6E_H       (REG_ADC_DTOP_BASE + 0xDD)
#define REG_ADC_DTOP_6F_L       (REG_ADC_DTOP_BASE + 0xDE)
#define REG_ADC_DTOP_6F_H       (REG_ADC_DTOP_BASE + 0xDF)
#define REG_ADC_DTOP_70_L       (REG_ADC_DTOP_BASE + 0xE0)
#define REG_ADC_DTOP_70_H       (REG_ADC_DTOP_BASE + 0xE1)
#define REG_ADC_DTOP_71_L       (REG_ADC_DTOP_BASE + 0xE2)
#define REG_ADC_DTOP_71_H       (REG_ADC_DTOP_BASE + 0xE3)
#define REG_ADC_DTOP_72_L       (REG_ADC_DTOP_BASE + 0xE4)
#define REG_ADC_DTOP_72_H       (REG_ADC_DTOP_BASE + 0xE5)
#define REG_ADC_DTOP_73_L       (REG_ADC_DTOP_BASE + 0xE6)
#define REG_ADC_DTOP_73_H       (REG_ADC_DTOP_BASE + 0xE7)
#define REG_ADC_DTOP_74_L       (REG_ADC_DTOP_BASE + 0xE8)
#define REG_ADC_DTOP_74_H       (REG_ADC_DTOP_BASE + 0xE9)
#define REG_ADC_DTOP_75_L       (REG_ADC_DTOP_BASE + 0xEA)
#define REG_ADC_DTOP_75_H       (REG_ADC_DTOP_BASE + 0xEB)
#define REG_ADC_DTOP_76_L       (REG_ADC_DTOP_BASE + 0xEC)
#define REG_ADC_DTOP_76_H       (REG_ADC_DTOP_BASE + 0xED)
#define REG_ADC_DTOP_77_L       (REG_ADC_DTOP_BASE + 0xEE)
#define REG_ADC_DTOP_77_H       (REG_ADC_DTOP_BASE + 0xEF)

#define REG_ADC_DTOP_FE_L       (REG_ADC_DTOP_BASE + 0xFF)    // for delay loop



// Bank ADC_ATOP
#define REG_ADC_ATOP_08         (0x08)
#define REG_ADC_ATOP_09         (0x09)

#define REG_ADC_ATOP_30         (0x30)

#endif

