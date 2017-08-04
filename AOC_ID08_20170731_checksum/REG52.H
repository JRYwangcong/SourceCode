#ifndef __REG52_H_
    #define __REG52_H_
    /*--------------------------------------------------------------------------
    REG52.H
    Header file for generic 80C52 and 80C32 microcontroller.
    Copyright (c) 1988-2001 Keil Elektronik GmbH and Keil Software, Inc.
    All rights reserved.
    --------------------------------------------------------------------------*/
    /*  BYTE Registers  */
    sfr P0 = 0x80;
    sfr P1 = 0x90;
    sfr P2 = 0xA0;
    sfr P3 = 0xB0;
    sfr PSW = 0xD0;
    sfr ACC = 0xE0;
    sfr B = 0xF0;
    sfr SP = 0x81;
    sfr DPL = 0x82;
    sfr DPH = 0x83;
    sfr PCON = 0x87;
    sfr TCON = 0x88;
    sfr TMOD = 0x89;
    sfr TL0 = 0x8A;
    sfr TL1 = 0x8B;
    sfr TH0 = 0x8C;
    sfr TH1 = 0x8D;
    sfr IE = 0xA8;
    sfr IP = 0xB8;
    sfr SCON = 0x98;
    sfr SBUF = 0x99;
    /*  8052 Extensions  */
    sfr T2CON = 0xC8;
    sfr RCAP2L = 0xCA;
    sfr RCAP2H = 0xCB;
    sfr TL2 = 0xCC;
    sfr TH2 = 0xCD;

    // 080717
    sfr WDTCON = 0xD8;
    sfr CKCON = 0x8E;

	
    /*  BIT Registers  */
    /*  PSW  */
    sbit CY = PSW ^ 7;
    sbit AC = PSW ^ 6;
    sbit F0 = PSW ^ 5;
    sbit RS1 = PSW ^ 4;
    sbit RS0 = PSW ^ 3;
    sbit OV = PSW ^ 2;
    sbit P = PSW ^ 0; //8052 only
    /*  TCON  */
    sbit TF1 = TCON ^ 7;
    sbit TR1 = TCON ^ 6;
    sbit TF0 = TCON ^ 5;
    sbit TR0 = TCON ^ 4;
    sbit IE1 = TCON ^ 3;
    sbit IT1 = TCON ^ 2;
    sbit IE0 = TCON ^ 1;
    sbit IT0 = TCON ^ 0;
    /*  IE  */
    sbit EA = IE ^ 7;
    sbit ET2 = IE ^ 5; //8052 only
    sbit ES = IE ^ 4;
    sbit ET1 = IE ^ 3;
    sbit EX1 = IE ^ 2;
    sbit ET0 = IE ^ 1;
    sbit EX0 = IE ^ 0;
    /*  IP  */
    sbit PT2 = IP ^ 5;
    sbit PS = IP ^ 4;
    sbit PT1 = IP ^ 3;
    sbit PX1 = IP ^ 2;
    sbit PT0 = IP ^ 1;
    sbit PX0 = IP ^ 0;
    /*  P3  */
    sbit RD = P3 ^ 7;
    sbit WR = P3 ^ 6;
    sbit T1 = P3 ^ 5;
    sbit T0 = P3 ^ 4;
    sbit INT1 = P3 ^ 3;
    sbit INT0 = P3 ^ 2;
    sbit TXD = P3 ^ 1;
    sbit RXD = P3 ^ 0;
    /*  SCON  */
    sbit SM0 = SCON ^ 7;
    sbit SM1 = SCON ^ 6;
    sbit SM2 = SCON ^ 5;
    sbit REN = SCON ^ 4;
    sbit TB8 = SCON ^ 3;
    sbit RB8 = SCON ^ 2;
    sbit TI = SCON ^ 1;
    sbit RI = SCON ^ 0;
    /*  P1  */
    sbit T2EX = P1 ^ 1; // 8052 only
    sbit T2 = P1 ^ 0; // 8052 only
    /*  T2CON  */
    sbit TF2 = T2CON ^ 7;
    sbit EXF2 = T2CON ^ 6;
    sbit RCLK = T2CON ^ 5;
    sbit TCLK = T2CON ^ 4;
    sbit EXEN2 = T2CON ^ 3;
    sbit TR2 = T2CON ^ 2;
    sbit C_T2 = T2CON ^ 1;
    sbit CP_RL2 = T2CON ^ 0;
    sbit P0_0 = P0 ^ 0;
    sbit P0_1 = P0 ^ 1;
    sbit P0_2 = P0 ^ 2;
    sbit P0_3 = P0 ^ 3;
    sbit P0_4 = P0 ^ 4;
    sbit P0_5 = P0 ^ 5;
    sbit P0_6 = P0 ^ 6;
    sbit P0_7 = P0 ^ 7;
    sbit P1_0 = P1 ^ 0;
    sbit P1_1 = P1 ^ 1;
    sbit P1_2 = P1 ^ 2;
    sbit P1_3 = P1 ^ 3;
    sbit P1_4 = P1 ^ 4;
    sbit P1_5 = P1 ^ 5;
    sbit P1_6 = P1 ^ 6;
    sbit P1_7 = P1 ^ 7;
    sbit P2_0 = P2 ^ 0;
    sbit P2_1 = P2 ^ 1;
    sbit P2_2 = P2 ^ 2;
    sbit P2_3 = P2 ^ 3;
    sbit P2_4 = P2 ^ 4;
    sbit P2_5 = P2 ^ 5;
    sbit P2_6 = P2 ^ 6;
    sbit P2_7 = P2 ^ 7;
    sbit P3_0 = P3 ^ 0;
    sbit P3_1 = P3 ^ 1;
    sbit P3_2 = P3 ^ 2;
    sbit P3_3 = P3 ^ 3;
    sbit P3_4 = P3 ^ 4;
    sbit P3_5 = P3 ^ 5;
    sbit P3_6 = P3 ^ 6;
    sbit P3_7 = P3 ^ 7;
    // For Winbond MCU
    sfr CHPENR = 0xF6; //for Winbond Mcu to the Isp mode
    sfr CHPCON = 0xBF; //for Winbond Mcu to the Isp mode
    sfr P4 = 0xEB;
    /*
    #if MST_MCU
    #define	ISPADD		xfr_regs[0x00]//(((BYTE xdata*)XFR_ADDR)[0x00])
    #define	ISPPW1		xfr_regs[0x01]//(((BYTE xdata*)XFR_ADDR)[0x01])
    #define	ISPPW2		xfr_regs[0x02]//(((BYTE xdata*)XFR_ADDR)[0x02])
    #define	ISPPW3		xfr_regs[0x03]//(((BYTE xdata*)XFR_ADDR)[0x03])
    #define	ISPPW4		xfr_regs[0x04]//(((BYTE xdata*)XFR_ADDR)[0x04])
    #define	ISPPW5		xfr_regs[0x05]//(((BYTE xdata*)XFR_ADDR)[0x05])
    #define	SW_EN_ISP	xfr_regs[0x06]//(((BYTE xdata*)XFR_ADDR)[0x06])
    #define	WD_SWZ1	    xfr_regs[0x08]//(((BYTE xdata*)XFR_ADDR)[0x08])
    #define	WD_SWZ2	    xfr_regs[0x09]//(((BYTE xdata*)XFR_ADDR)[0x09])
    #define	SERIAL_ID		xfr_regs[0x0A]//(((BYTE xdata*)XFR_ADDR)[0x0A])
    #define	INTMASK_EN_ADC	xfr_regs[0x10]//(((BYTE xdata*)XFR_ADDR)[0x10])
    #define	ADC_INT_FLAG	xfr_regs[0x11]//(((BYTE xdata*)XFR_ADDR)[0x11])
    #define	ADC_WBUF_RDP	xfr_regs[0x12]//(((BYTE xdata*)XFR_ADDR)[0x12])
    #define	ADC_RBUF_WDP	xfr_regs[0x13]//(((BYTE xdata*)XFR_ADDR)[0x13])
    #define	INMASK_EN_DVI	xfr_regs[0x14]//(((BYTE xdata*)XFR_ADDR)[0x14])
    #define	DDC2BI_CTRL	    xfr_regs[0x18]//(((BYTE xdata*)XFR_ADDR)[0x18])
    #define	KEYPAD_ADC1	    xfr_regs[0x20]//(((BYTE xdata*)XFR_ADDR)[0x20])
    #define	KEYPAD_ADC2	    xfr_regs[0x21]//(((BYTE xdata*)XFR_ADDR)[0x21])
    #define	KEYPAD_ADC3	    xfr_regs[0x22]//(((BYTE xdata*)XFR_ADDR)[0x22])
    #define	KEYPAD_ADC4	    xfr_regs[0x23]//(((BYTE xdata*)XFR_ADDR)[0x23])
    #define	MCU_P0_OUT_EN	xfr_regs[0x30]//(((BYTE xdata*)XFR_ADDR)[0x30])
    #define	MCU_P0_FMD		xfr_regs[0x31]//(((BYTE xdata*)XFR_ADDR)[0x31])
    #define	MCU_P0_ROD_EN	xfr_regs[0x32]//(((BYTE xdata*)XFR_ADDR)[0x32])
    #define	MCU_P1_OUT_EN	xfr_regs[0x33]//(((BYTE xdata*)XFR_ADDR)[0x33])
    #define	MCU_P1_FMD		xfr_regs[0x34]//(((BYTE xdata*)XFR_ADDR)[0x34])
    #define	MCU_P1_ROD_EN	xfr_regs[0x35]//(((BYTE xdata*)XFR_ADDR)[0x35])
    #define	MCU_P2_OUT_EN	xfr_regs[0x36]//(((BYTE xdata*)XFR_ADDR)[0x36])
    #define	MCU_P2_FMD		xfr_regs[0x37]//(((BYTE xdata*)XFR_ADDR)[0x37])
    #define	MCU_P2_ROD_EN	xfr_regs[0x38]//(((BYTE xdata*)XFR_ADDR)[0x38])
    #define	MCU_P3_OUT_EN	xfr_regs[0x39]//(((BYTE xdata*)XFR_ADDR)[0x39])
    #define	MCU_P3_FMD		xfr_regs[0x3A]//(((BYTE xdata*)XFR_ADDR)[0x3A])
    #define	MCU_P3_ROD_EN	xfr_regs[0x3B]//(((BYTE xdata*)XFR_ADDR)[0x3B])
    #define	MCU_P4_OUT_EN	xfr_regs[0x3C]//(((BYTE xdata*)XFR_ADDR)[0x3C])
    #define	MCU_P4_FMD		xfr_regs[0x3D]//(((BYTE xdata*)XFR_ADDR)[0x3D])
    #define	MCU_P4_ROD_EN	xfr_regs[0x3E]//(((BYTE xdata*)XFR_ADDR)[0x3E])
    #define	SPI_SI_BUF				((BYTE xdata*)0xC040)
    #define	SPI_SO_BUF				((BYTE xdata*)0xC049)
    #define	SPI_CTRL					(((BYTE xdata*)0xC048)[0])
    #endif
     */
    // #if MSTMCU
    #define ISPADD		xfr_regs[0x00]//(((BYTE xdata*)XFR_ADDR)[0x00])
    #define ISPPW1		xfr_regs[0x01]//(((BYTE xdata*)XFR_ADDR)[0x01])
    #define ISPPW2		xfr_regs[0x02]//(((BYTE xdata*)XFR_ADDR)[0x02])
    #define ISPPW3		xfr_regs[0x03]//(((BYTE xdata*)XFR_ADDR)[0x03])
    #define ISPPW4		xfr_regs[0x04]//(((BYTE xdata*)XFR_ADDR)[0x04])
    #define ISPPW5		xfr_regs[0x05]//(((BYTE xdata*)XFR_ADDR)[0x05])
    //#define	RCATXABUF	(((BYTE xdata*)XFR_ADDR)[0x06])
    #define SW_EN_ISP		xfr_regs[0x06]//(((BYTE xdata*)XFR_ADDR)[0x06])
    //#define	SLVAADDR	(((BYTE xdata*)XFR_ADDR)[0x07])
    //#define	RCBTXBBUF	(((BYTE xdata*)XFR_ADDR)[0x08])
    //#define	SLVBADDR	(((BYTE xdata*)XFR_ADDR)[0x09])
    #define WD_SWZ1	xfr_regs[0x08]//(((BYTE xdata*)XFR_ADDR)[0x08])
    #define WD_SWZ2	xfr_regs[0x09]//(((BYTE xdata*)XFR_ADDR)[0x09])
    #define SERIAL_ID		xfr_regs[0x0A]//(((BYTE xdata*)XFR_ADDR)[0x0A])
    //#define	ISPSLV	(((BYTE xdata*)XFR_ADDR)[0x0B])
    //#define	ISPEN	(((BYTE xdata*)XFR_ADDR)[0x0C])
    #define INTMASK_EN_ADC	xfr_regs[0x10]//(((BYTE xdata*)XFR_ADDR)[0x10])
    #define ADC_INT_FLAG	xfr_regs[0x11]//(((BYTE xdata*)XFR_ADDR)[0x11])
    #define ADC_WBUF_RDP	xfr_regs[0x12]//(((BYTE xdata*)XFR_ADDR)[0x12])
    #define ADC_RBUF_WDP	xfr_regs[0x13]//(((BYTE xdata*)XFR_ADDR)[0x13])
    #define INTMASK_EN_DVI	xfr_regs[0x14]//(((BYTE xdata*)XFR_ADDR)[0x14])
    #define DVI_INT_FLAG	xfr_regs[0x15]//(((BYTE xdata*)XFR_ADDR)[0x11])
    #define DVI_WBUF_RDP	xfr_regs[0x16]//(((BYTE xdata*)XFR_ADDR)[0x12])
    #define DVI_RBUF_WDP	xfr_regs[0x17]//(((BYTE xdata*)XFR_ADDR)[0x13])
    #define DDC2BI_CTRL	xfr_regs[0x18]//(((BYTE xdata*)XFR_ADDR)[0x18])
    #define ADC_DDCCI_ID	xfr_regs[0x19]//(((BYTE xdata*)XFR_ADDR)[0x18])
    #define DVI_DDCCI_ID	xfr_regs[0x1A]//(((BYTE xdata*)XFR_ADDR)[0x18])
    #define KEYPAD_ADC1	xfr_regs[0x20]//(((BYTE xdata*)XFR_ADDR)[0x20])
    #define KEYPAD_ADC2	xfr_regs[0x21]//(((BYTE xdata*)XFR_ADDR)[0x21])
    #define KEYPAD_ADC3	xfr_regs[0x22]//(((BYTE xdata*)XFR_ADDR)[0x22])
    #define KEYPAD_ADC4	xfr_regs[0x23]//(((BYTE xdata*)XFR_ADDR)[0x23])
    #define MCU_P0_OUT_EN	xfr_regs[0x30]//(((BYTE xdata*)XFR_ADDR)[0x30])
    #define MCU_P0_FMD			xfr_regs[0x31]//(((BYTE xdata*)XFR_ADDR)[0x31])
    #define MCU_P0_ROD_EN	xfr_regs[0x32]//(((BYTE xdata*)XFR_ADDR)[0x32])
    #define MCU_P1_OUT_EN	xfr_regs[0x33]//(((BYTE xdata*)XFR_ADDR)[0x33])
    #define MCU_P1_FMD			xfr_regs[0x34]//(((BYTE xdata*)XFR_ADDR)[0x34])
    #define MCU_P1_ROD_EN	xfr_regs[0x35]//(((BYTE xdata*)XFR_ADDR)[0x35])
    #define MCU_P2_OUT_EN	xfr_regs[0x36]//(((BYTE xdata*)XFR_ADDR)[0x36])
    #define MCU_P2_FMD			xfr_regs[0x37]//(((BYTE xdata*)XFR_ADDR)[0x37])
    #define MCU_P2_ROD_EN	xfr_regs[0x38]//(((BYTE xdata*)XFR_ADDR)[0x38])
    #define MCU_P3_OUT_EN	xfr_regs[0x39]//(((BYTE xdata*)XFR_ADDR)[0x39])
    #define MCU_P3_FMD			xfr_regs[0x3A]//(((BYTE xdata*)XFR_ADDR)[0x3A])
    #define MCU_P3_ROD_EN	xfr_regs[0x3B]//(((BYTE xdata*)XFR_ADDR)[0x3B])
    #define MCU_P4_OUT_EN	xfr_regs[0x3C]//(((BYTE xdata*)XFR_ADDR)[0x3C])
    #define MCU_P4_FMD			xfr_regs[0x3D]//(((BYTE xdata*)XFR_ADDR)[0x3D])
    #define MCU_P4_ROD_EN	xfr_regs[0x3E]//(((BYTE xdata*)XFR_ADDR)[0x3E])
    #define SPI_SI_BUF		((BYTE xdata*)0xC040)
    #define SPI_SO_BUF		((BYTE xdata*)0xC049)
    #define SPI_CTRL			(((BYTE xdata*)0xC048)[0])
    #define INTSTA1     xfr_regs[0x90]
    #define INTENA1     xfr_regs[0x91]
    #define INTINV1     xfr_regs[0x92]
    #define INTCTRL1    xfr_regs[0x93]

    #define MCU_CACHE_CTL  	xfr_regs[0x60]
    #define GT_mode                  xfr_regs[0xDF]
    #define WD_CNT_EXT    xfr_regs[0xDF]
	
#if 1//def TSUMW88MDT3_For_HDMIDDCCI
#define	INTMASK_EN_ADC1	        xfr_regs[0x70] //(((BYTE xdata*)XFR_ADDR)[0x70])
#define	ADC1_INT_FLAG		        xfr_regs[0x71] //(((BYTE xdata*)XFR_ADDR)[0x71])
#define	ADC1_WBUF_RDP	        xfr_regs[0x72] //(((BYTE xdata*)XFR_ADDR)[0x72])
#define	ADC1_RBUF_WDP	        xfr_regs[0x73] //(((BYTE xdata*)XFR_ADDR)[0x73])
#define	INTMASK_EN_DVI1	        xfr_regs[0x74] //(((BYTE xdata*)XFR_ADDR)[0x74])
#define	DVI1_INT_FLAG		        xfr_regs[0x75] //(((BYTE xdata*)XFR_ADDR)[0x75])
#define	DVI1_WBUF_RDP		        xfr_regs[0x76] //(((BYTE xdata*)XFR_ADDR)[0x76])
#define	DVI1_RBUF_WDP		        xfr_regs[0x77] //(((BYTE xdata*)XFR_ADDR)[0x77])
#define	ADC1_DDCCI_ID		        xfr_regs[0x79] //(((BYTE xdata*)XFR_ADDR)[0x79])
#define	DVI1_DDCCI_ID		        xfr_regs[0x7A] //(((BYTE xdata*)XFR_ADDR)[0x7A])
#endif

    // #endif MSTMCU
#endif
