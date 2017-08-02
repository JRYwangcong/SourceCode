
// CHIP_FAMILY_MST9U
// interrupt
#define INT_MASK_ALL()                (msWrite2Byte(REG_002B08, 0xFFFF), msWrite2Byte(REG_002B18, 0xFFFF), msWrite2Byte(REG_002B28, 0xFFFF))
#define INT_STATUS_HBR2_ISR()           (_bit6_( MEM_MSREAD_BYTE(REG_163070)))
#define INT_IRQ_D2B_ENABLE(Enable)      (msWriteByteMask(REG_002B28, (Enable)?(0):(BIT2), BIT2))
#define INT_STATUS_D2B_ISR()            (_bit2_( MEM_MSREAD_BYTE(REG_002B38)))
#define INT_FIQ_IR_ENABLE(Enable)       (msWriteByteMask(REG_002B08, (Enable)?(0):(BIT3), BIT3))
#define INT_FIQ_IR_RC_ENABLE(Enable)    (msWriteByteMask(REG_002B08, (Enable)?(0):(BIT3), BIT3))
#define INT_STATUS_IR_ISR()             (_bit3_( MEM_MSREAD_BYTE(REG_002B18)))
#define INT_CLEAR_STATUS_IR_ISR()       (msWriteByte(REG_002B18, msReadByte(REG_002B18)))
//#define INT_STATUS_LEDSARINT_ISR()               (_bit7_(_MEM_MSREAD_BYTE(REG_2B28)))
//#define INT_SCALER_A(Enable) (_msWriteByte(SC0_CE, Enable))
//#define INT_SCALER_B(Enable) (_msWriteByte(SC0_CF, Enable))
//#define INT_STATUS_CLEAR() (_msWriteByte(SC0_CC, 0), _msWriteByte(SC0_CD, 0))
//#define INT_SCALER_A_ISR(Enable) (_MEM_SCWRITE_BYTE(SC0_CE, Enable))
//#define INT_SCALER_B_ISR(Enable) (_MEM_SCWRITE_BYTE(SC0_CF, Enable))
//#define INT_STATUS_A_ISR(Status) (_MEM_SCREAD_BYTE(SC0_CC) & (Status))
//#define INT_STATUS_B_ISR(Status) (_MEM_SCREAD_BYTE(SC0_CD) & (Status))
//#define INT_STATUS_CLEAR_ISR() (_MEM_SCWRITE_BYTE(SC0_CC, 0), _MEM_SCWRITE_BYTE(SC0_CD, 0))

#if UART1
#define UART_READ_ES()  ((IEN2&ES1) == ES1)
#define UART_RI_FLAG()  ((S1CON & RI1) == RI1)
#define UART_TI_FLAG()  ((S1CON & TI1) == TI1)
#define UART_CLR_RI_FLAG()  (S1CON &= ~RI1)
#define UART_CLR_TI_FLAG()  (S1CON &= ~TI1)
#define UART_BUFFER()  (S1BUF)
#else
#define UART_READ_ES()  (ES)
#define UART_RI_FLAG()  (RI)
#define UART_TI_FLAG()  (TI)
#define UART_CLR_RI_FLAG()  (RI = 0)
#define UART_CLR_TI_FLAG()  (TI = 0)
#define UART_BUFFER()  (SBUF)
#endif

// cache
//#define CACHE_STATUS()   (_bit3_(_msReadByte(REG_1018)))
//#define CACHE_ENABLE()  (_msWriteBit( REG_1018, TRUE, _BIT3 ), _msWriteBit( REG_2BA0, FALSE, _BIT0 ))
//#define CACHE_DISABLE()  (_msWriteBit( REG_2BA0, TRUE, _BIT0 ), _msWriteBit( REG_1018, FALSE, _BIT3 ))

// watch dog
//#define WDT_ENABLE()    (_msWriteByte( REG_3C60, 0xAA ), _msWriteByte( REG_3C61, 0x55 ))
//#define WDT_DISABLE()    (_msWriteByte( REG_3C60, 0x55 ), _msWriteByte( REG_3C61, 0xAA ))
//#define WDT_INTERVAL(Value) (_msWriteByte( REG_3C62, LOBYTE( Value ) ), _msWriteByte( REG_3C63, HIBYTE( Value ) ))
//#define WDT_CLEAR()     (_msWriteBit( REG_3C66, 1, _BIT1 ))

// dvi
//#define DVI_DE_STABLE()  (  (_msReadByte( REG_155D) & (BIT6|BIT5))== BIT6) // DE stable
//#define DVI_CLK_STABLE()  (  (_msReadByte( REG_165C) & (BIT3))== BIT3) // CLK stable
#define DVI_CLOCK_IS_MISSING()  0//((_msReadByte(SC0_CA)&BIT6) == BIT6)

// scaler
#define SC0_SCALER_RESET(Value, ScOffset)  (msWriteByte( SC00_04+ScOffset, Value ))
#define SC0_SET_OUTPUT_VTOTAL(Value, ScOffset) (msWrite2ByteMask(SC10_1A+ScOffset, u16Value, SC_MASK_OP2_V))
#define SC0_SET_AUTO_HTOTAL(Enable, ScOffset)   (msWriteByteMask(SC10_21+ScOffset, (Enable)?(BIT1):(0) , BIT1))
#define SC0_WHITE_SCREEN_ENABLE()  (msWriteByteMask(SCL0_10_43,BIT5,(BIT4|BIT5)))
#define SC0_BLACK_SCREEN_ENABLE()  (msWriteByteMask(SCL0_10_43,BIT4,(BIT4|BIT5)))
#define SC0_BLACK_WHITE_SCREEN_DISABLE()  (msWriteByteMask(SCL0_10_43,0,(BIT4|BIT5)))

//#define SC0_READ_POWER_DOWN_STATUS()    (_msReadByte(SC0_F0))
//#define SC0_READ_ADC_COAST_ENABLE_VALUE()   (_msReadByte(SC0_ED))
//#define SC0_READ_ADC_COAST_START_VALUE()   (_msReadByte(SC0_EE))
//#define SC0_READ_ADC_COAST_END_VALUE()   (_msReadByte(SC0_EF))
//#define SC0_SAMPLE_CLOCK_INVERT(Value)   (_msWriteByteMask(SC0_F3, ((Value)>165)?(BIT0):(0), BIT0))
//#define SC0_VSYNC_WIDTH_REPORT(Enable)  (_msWriteByteMask(SC0_FA, (Enable)?(BIT0):(0),BIT0))

#ifdef _NEW_SOG_DET_
#define NEW_SOG_DETECT_FLUSH()  (msWriteByteMask(REG_ADC_PMATOP_7A_L, BIT3, BIT3), msWriteByteMask(REG_ADC_PMATOP_7A_L, 0, BIT3))
#endif

#define OSD_MENU_EXIST()    ( (msReadByte( (OSD_MAIN_WND<<5)+OSD2_00 )&MWIN_B) == MWIN_B ) // V

// misc
#define ADC_PLL_LOCKING_EDGE(TrailingEdge)  (msWriteByteMask(REG_ADC_DTOP_07_L, (TrailingEdge)?(BIT5):(0), BIT5))

//OSD
//Code/Attr RAM bit8/bit9
#define OSD_TEXT_HI_ADDR_SET_BIT8()     g_u8OsdFontDataHighByte = BIT0//enable bit 8
#define OSD_TEXT_HI_ADDR_SET_BIT9()     g_u8OsdFontDataHighByte = BIT1 //enable bit 9
#define OSD_TEXT_HI_ADDR_CLR_TO_0()     g_u8OsdFontDataHighByte = 0x00

#define PORT_CODE_ADDR  OSD1_66
#define PORT_CODE_DATA  OSD1_66
#define PORT_ATTR_ADDR  OSD1_66
#define PORT_ATTR_DATA  OSD1_66
#define PORT_FONT_ADDR  OSD1_66
#define PORT_FONT_DATA  OSD1_66

#define WRITE_CAFSRAM_ADDR()            msWriteByteMask(OSD1_68, 0, 0x1F)
#if (CHIP_ID == MST9U3)
#define WRITE_CODE()                    (msWriteByteMask(OSD1_68, 1, 0x1F),\
                                        msWriteByteMask(OSD1_67, g_u8OsdFontDataHighByte, BIT3|BIT2|BIT1|BIT0))
#else
#define WRITE_CODE()                    (msWriteByteMask(OSD1_68, 1, 0x1F),\
                                        msWriteByteMask(OSD1_67, g_u8OsdFontDataHighByte, BIT1|BIT0))
#endif
#define WRITE_ATTRIBUTE()               msWriteByteMask(OSD1_68, 2, 0x1F)
#define WRITE_FONT()                    msWriteByteMask(OSD1_68, 3, 0x1F)
#define CAFSRAM_ERASE_TRIG()            msWriteByteMask(OSD1_68, 4, 0x1F)
#define CAFSRAM_READ_TRIG()             msWriteByteMask(OSD1_68, 5, 0x1F)

#define WRITE_PSRAM0_ADDR()             msWriteByteMask(OSD1_68, 10, 0x1F)
#define WRITE_PSRAM0_MASK()             msWriteByteMask(OSD1_68, 11, 0x1F)
#define WRITE_PSRAM0_COLOR_KEY()        msWriteByteMask(OSD1_68, 12, 0x1F)
#define WRITE_PSRAM0_COLOR()            msWriteByteMask(OSD1_68, 13, 0x1F)
#define PSRAM0_ERASE_TRIG()             msWriteByteMask(OSD1_68, 14, 0x1F)
#define PSRAM0_READ_TRIG()              msWriteByteMask(OSD1_68, 15, 0x1F)

#define WRITE_PSRAM1_ADDR()             msWriteByteMask(OSD1_68, 20, 0x1F)
#define WRITE_PSRAM1_COLOR_KEY()        msWriteByteMask(OSD1_68, 21, 0x1F)
#define WRITE_PSRAM1_COLOR()            msWriteByteMask(OSD1_68, 22, 0x1F)
#define PSRAM1_ERASE_TRIG()             msWriteByteMask(OSD1_68, 23, 0x1F)
#define PSRAM1_READ_TRIG()              msWriteByteMask(OSD1_68, 24, 0x1F)


#define OSD_WRITE_FONT_ADDRESS(u8Addr)  {WRITE_CAFSRAM_ADDR();msWrite2Byte(OSD1_66, GET_FONT_RAM_ADDR(u8Addr));}

#define SET_OSD_CTRL_REG_WR_PORT(V)     {msWriteByte(OSD1_66, (V)); msWriteByteMask(OSD1_67,((V)>>8),0x3F);}

#define WR_FONT_MODE_MONO   0
#define WR_FONT_MODE_2BIT   0
#define WR_FONT_MODE_3BIT   1
#define WR_FONT_MODE_4BIT   2
#define OSD_SET_WRITE_FONT_MODE(u8Mode) msWriteByteMask(OSD1_6D,((u8Mode)<<6),BIT7|BIT6)

//============================================================
// Keypad sar status
//============================================================
#define KEYPAD_SAR00         MEM_MSREAD_BYTE(REG_003A20)//(((BYTE xdata*)XFR_ADDR)[0x20])
#define KEYPAD_SAR01         MEM_MSREAD_BYTE(REG_003A21)//(((BYTE xdata*)XFR_ADDR)[0x21])
#define KEYPAD_SAR02         MEM_MSREAD_BYTE(REG_003A22)//(((BYTE xdata*)XFR_ADDR)[0x22])
#define KEYPAD_SAR03         MEM_MSREAD_BYTE(REG_003A23)//(((BYTE xdata*)XFR_ADDR)[0x23])
#define KEYPAD_SAR04         MEM_MSREAD_BYTE(REG_003A26)
#define KEYPAD_SAR05         MEM_MSREAD_BYTE(REG_003A27)
#define KEYPAD_SAR06         MEM_MSREAD_BYTE(REG_003A28)
#define KEYPAD_SAR07         MEM_MSREAD_BYTE(REG_003A29)
#define KEYPAD_SAR08         MEM_MSREAD_BYTE(REG_003A2A)
#define KEYPAD_SAR09         MEM_MSREAD_BYTE(REG_003A2B)
#define KEYPAD_SAR10         MEM_MSREAD_BYTE(REG_003A2C)
#define KEYPAD_SAR11         MEM_MSREAD_BYTE(REG_003A2D)
