#include "global.h"
#ifndef _DEBUG_H_
#define  _DEBUG_H_

#ifdef _DEBUG_C_
  #define _DEBUGDEC_
#else
  #define _DEBUGDEC_ extern
#endif

#if ENABLE_DEBUG
#define DEBUG_PRINT_ENABLE                   	1//ENABLE_DEBUG//For printf,putchar and putstr
#define CABLE_DETECT_DEBUG_ENABLE    	0
#define ADC_KEY_PRESS_DEBUG_ENABLE	0
#define DDCCI_DEBUG_ENABLE              	0
#define DEBUG_SCANKEY       				0
#define DEBUG_OSD                                       0
#define DEBUG_MCU_GPIO                             0
#define g_bDebugASCIICommandFlag    0
#else
#define DEBUG_PRINT_ENABLE                   	0 //For printf,putchar and putstr
#define CABLE_DETECT_DEBUG_ENABLE    	0
#define ADC_KEY_PRESS_DEBUG_ENABLE	0
#define DDCCI_DEBUG_ENABLE              	0
#define DEBUG_SCANKEY       				0
#define DEBUG_OSD                                       0
#define DEBUG_MCU_GPIO                             0
#define g_bDebugASCIICommandFlag    0
#endif

_DEBUGDEC_ void putSIOChar( unsigned char sendData );
_DEBUGDEC_ void ProcessCommand( void );
_DEBUGDEC_ Bool GetCommand( void );
_DEBUGDEC_ void DebugHandler( void );

_DEBUGDEC_ void putSIOChar_UART1(unsigned char sendData);
_DEBUGDEC_ void ProcessCommand_UART1( void );
_DEBUGDEC_ Bool GetCommand_UART1( void );
_DEBUGDEC_ void UART1_Handler(void);

#define SIO_ID1     SIORxBuffer[0]
#define SIO_TYPE1   SIORxBuffer[1]
#define SIO_TYPE2   SIORxBuffer[2]
#define SIO_CMD1    SIORxBuffer[3]
#define SIO_CMD2    SIORxBuffer[4]
#define SIO_DATA1   SIORxBuffer[5]
#define SIO_DATA2   SIORxBuffer[6]
#define SIO_DATA3       SIORxBuffer[7]
#define SIOData10   (SIO_DATA1-0x30)*256
#define SIOData11   ((SIO_DATA1-0x41)+10)*256
#define SIOData20   (SIO_DATA2-0x30)*16
#define SIOData21   ((SIO_DATA2-0x41)+10)*16
#define SIOData30   (SIO_DATA3-0x30)
#define SIOData31   (SIO_DATA3-0x41)+10

#if ENABLE_R2_INT_M51_Test
typedef enum
{
    hst3to0_int,
    hst3to1_int,
    hst3to2_int,
    hst2to0_int,
    hst2to1_int,
    hst2to3_int,
    hst1to0_int,
    hst1to2_int,
    hst1to3_int,
    hst0to1_int,
    hst0to2_int,
    hst0to3_int,
}NonPMFIQ_INT;
_DEBUGDEC_ void TestCommand_FIQ_INT(BYTE ucPara1);
#endif
#endif
