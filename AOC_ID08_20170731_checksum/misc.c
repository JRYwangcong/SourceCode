#include <intrins.h>
#include "types.h"
#include "board.h"
#include "global.h"
#if AutoProcess
#include "Menudef.h"   //zjb20071218
#include "MsOsd.h"
#include "Menu.h"
#endif
#define DelayPeriod ((WORD)CPU_CLOCK_MHZ*78/12)
void Delay4us(void)
{
   #if 0//Enable_Cache    // jeff 12 22 for cache add it 
   BYTE i;  

    for(i=0;i<50;i++) // 50 090421 100 090417 
    {
        _nop_();
        _nop_();
    }

   #else
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    #endif 
}
#if AutoProcess
//zjb20071218
void Menu_DrawAutoCount(void)
{
    //Peter.Yu 20120529 for auto show 101% issue(wAutoProcess0  updates in Time0Interrupt when we draw it).
	if(MenuPageIndex==AutoMenu)//||MenuPageIndex==ResetMenu)
	{
        WORD wAutoProcess0_temp;
        wAutoProcess0_temp = wAutoProcess0;
        if(wAutoProcess0_temp != wAutoProcess1 && wAutoProcess0_temp <= 100)
		{
			//Osd_Set256TextColor(4 ,Color_8);	
			Osd_SetTextColor(0, 14);
			Osd_DrawGuage(12, 5, 10, wAutoProcess0_temp);
			//Osd_Set256TextColor(4 ,Color_8);	
			Osd_DrawNum(21,5, wAutoProcess0_temp);
			wAutoProcess1=wAutoProcess0_temp;
		}
	}
}


#if 1//Enable_Cache

extern BYTE xdata rxStatus;
void Delay1ms(WORD msNums)
{
    if(!msNums)
        return;
    
    DelayCounter = msNums;
    bDelayFlag = 1;
    #if AutoProcess
    Menu_DrawAutoCount();
    #endif
    while(bDelayFlag)
    {
        if(InputTimingChangeFlag)
        {
            DelayCounter = 0;
            bDelayFlag = 0;
            break;
        }
    }
}

#endif
void ForceDelay1ms(WORD msNums)
{ 
    if(!msNums)
        return;
    
    DelayCounter = msNums;
    bDelayFlag = 1;
    while(bDelayFlag);
}
#else
void Delay1ms(WORD msNums)
{ WORD t;

  if (msNums>0)
    { while (msNums--)
        { t=DelayPeriod;
          while (t--);
        }
    }
}



void ForceDelay1ms(WORD msNums)
{ WORD t;

  if (msNums>0)
    { while (msNums--)
        { t=DelayPeriod;
          while (t--);
        }
    }
}
#endif

BYTE GetVSyncTime(void)
{
    WORD freq;
    freq = ((DWORD)MST_CLOCK_MHZ *10+SrcHPeriod / 2) / SrcHPeriod; //get hfreq round 5
    freq = ((WORD)SrcVTotal *10+(freq / 2)) / freq;
    return (BYTE)freq;
}
#if ENABLE_HDCP
	#if HDCPKEY_IN_Flash
		WORD CRC16(BYTE nData, WORD nCRC)
		{
			BYTE nMask;
			Bool fFeedback;
			for (nMask = 0x80; nMask; nMask >>= 1)
			{
				fFeedback = (Bool)(((nCRC &0x8000) ? 1 : 0) ^ ((nMask &nData) ? 1 : 0));
				nCRC <<= 1;
				if (fFeedback)
				{
					nCRC ^= 0x1021;
				}
			}
			return nCRC;
		}
	#endif
#endif

