/******************************************************************************
 Copyright (c) 2003 MStar Semiconductor, Inc.
 All rights reserved.

 [Module Name]: Buzzer.h
 [Date]:
 [Comment]:
   OSD menu relative subroutine.
 [Reversion History]:
*******************************************************************************/
#include "board.h"

#ifdef _BUZZER_C_
  #define _BUZZER_C_
#else
  #define _BUZZER_C_ extern
#endif

//_BUZZER_C_ void MuteBuzzer(void);
//_BUZZER_C_ void LowFrequencyBeep(void);
//_BUZZER_C_ void HighFrequencyBeep(void);

//#if (BOARD_TYPE_SEL ==  BD_DELL_AW2310)
_BUZZER_C_ void LowBeep(void);
_BUZZER_C_ void HighBeep(void);
_BUZZER_C_ void BuzzerMute(void);
//#endif
//#if (BOARD_TYPE_SEL == BD_DELL_2710W)
_BUZZER_C_ void BuzzerInitial(void);
//#endif
