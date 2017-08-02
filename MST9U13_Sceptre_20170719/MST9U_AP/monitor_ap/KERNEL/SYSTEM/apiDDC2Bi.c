
#define _APIDDC2BI_C_
#include <string.h>
#include "Global.h"
#include "apiDDC2Bi.h"

#define APIDDC2BI_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && APIDDC2BI_DEBUG
#define APIDDC2BI_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define APIDDC2BI_PRINT(format, ...)
#endif



/////////////////////////////////////////////////////////////////////////
////////////////// DDC2Bi enable/disable for each port  ///////////////////////
/////////////////////////////////////////////////////////////////////////
void msAPI_DDC2BiEnable(BYTE u8InputPort, BOOL bEnable)
{
    if(INPUT_IS_MUX_ADC(u8InputPort))
        D2B_SET_ID_VGA(bEnable);
    else if(INPUT_IS_MUX_C0(u8InputPort))
        D2B_SET_ID_DVI0(bEnable);
    else if(INPUT_IS_MUX_C1(u8InputPort))
        D2B_SET_ID_DVI1(bEnable);
    else if(INPUT_IS_MUX_C2(u8InputPort))
        D2B_SET_ID_DVI2(bEnable);
    else if(INPUT_IS_MUX_C3(u8InputPort))
        D2B_SET_ID_DVI3(bEnable);
    else if(INPUT_IS_MUX_C4(u8InputPort))
        D2B_SET_ID_DVI4(bEnable);
    else //if(INPUT_IS_MUX_C5(u8InputPort))
        D2B_SET_ID_DVI5(bEnable);
}



