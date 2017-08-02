#ifndef _MCU_H_
#define _MCU_H_

#ifdef _MCU_C_
#define _MCUDEC_
#else
#define _MCUDEC_    extern
#endif


_MCUDEC_ void mStar_InitMCU( void );
_MCUDEC_ void Init_Interrupt( void );
_MCUDEC_ void Init_IOPorts( void );
_MCUDEC_ void msDrvVsyncINTEnable( void );
_MCUDEC_ void msDrvIPVsyncINTEnable( Bool bEnable );
_MCUDEC_ void msDrvATPINTEnable( Bool bEnable );
#endif
