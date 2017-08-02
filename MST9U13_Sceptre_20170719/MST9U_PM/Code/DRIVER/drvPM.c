
#include "Global.h"
#include "Board.h"
#include "Model_Define.h"

#if CHIP_ID == MST9U||CHIP_ID == MST9U2
#include "..\MST9U\drvPM_MST9U.c"
#elif CHIP_ID == MST9U3
#include "..\MST9U3\drvPM_MST9U3.c"
#else
#message "please implement DrvPM.c for new chip"
#endif

#if 0
BYTE code msDACNullData[] = {0};
void msDACDummy(void)
{
    BYTE xdata i = msDACNullData[0];
}
#endif
