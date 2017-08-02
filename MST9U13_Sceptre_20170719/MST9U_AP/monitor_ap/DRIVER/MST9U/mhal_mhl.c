///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mhal_mhl.c
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_MHL_C_
#define _MHAL_MHL_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "Global.h"
#include "mapi_mhl.h"
#include "mdrv_mhl.h"
#include "mhal_mhl.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MHL_HAL_DEBUG_MESSAGE           0

#if(MHL_DEBUG_MESSAGE && MHL_HAL_DEBUG_MESSAGE)
#define MHL_HAL_DPUTSTR(str)            printMsg(str)
#define MHL_HAL_DPRINTF(str, x)         printData(str, x)
#else
#define MHL_HAL_DPUTSTR(str)
#define MHL_HAL_DPRINTF(str, x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct
{
    WORD addr;
    WORD databuf;
    WORD mask;
} msLoadTbl_S;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
Bool bSignalStable[6] = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};
BYTE ucCableDetectEnable = 0;

//-------------------------------------------------------------------------------------------------
//  MHL power on table
//-------------------------------------------------------------------------------------------------
code msLoadTbl_S tMHL_POWER_ON_TABLE[] =
{
    {REG_0BD8, BIT14, BIT14}, // REG_MHL_CBUS_6C[14]: Register xdata enable
    {REG_0C02, MASKBIT(5:2), 0}, // REG_PM_MHL_CBUS_0C[5][3][2]
};

//-------------------------------------------------------------------------------------------------
//  MHL power down table
//-------------------------------------------------------------------------------------------------
code msLoadTbl_S tMHL_POWER_DOWN_TABLE[] =
{
    {REG_0260, MASKBIT(1:0), MASKBIT(1:0)}, // REG_PM_GPIO_30[1:0], CBUS port selsct [1:0]
    {REG_0262, BIT2 |BIT3 |BIT5, BIT2 |BIT3 |BIT5}, // REG_PM_GPIO_31[5][3][2],  Clear all PE_CTRL
    {REG_0BD8, BIT14, 0}, // REG_MHL_CBUS_6C[14]: Register xdata enable
    {REG_0C02, MASKBIT(5:2), BIT5 |BIT3 |BIT2}, // REG_PM_MHL_CBUS_0C[5][3][2]
};

//-------------------------------------------------------------------------------------------------
//  MHL power saving table
//-------------------------------------------------------------------------------------------------
code msLoadTbl_S tMHL_POWER_SAVING_TABLE[] =
{
    {REG_0BD8, BIT14, BIT14}, // REG_MHL_CBUS_6C[14]: Register xdata enable // 130220 coding test
};

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CbusPadSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_CbusPadSetting(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            // Cbus pad config
            //msWrite2ByteMask(REG_000462, 0, BIT0); // REG_PAD_TOP_31[0], enable pad pull-high port0
            msWrite2ByteMask(REG_002F62, BIT11, BIT11); // REG_PM_MHL_CBUS0_31[11]
            // Set accepted discover pulse high pulse width to ignore USB pulse
            msWrite2ByteMask(REG_002F18, 0x0046, MASKBIT(15:0)); // REG_PM_MHL_CBUS0_0C[15:0], reject cbus discovery pulse below this limit
            msWrite2ByteMask(REG_002F1A, 0x0082, MASKBIT(15:0)); // REG_PM_MHL_CBUS0_0D[15:0], reject cbus discovery pulse above this limit
            // CTS 4.3.7.1
            msWrite2ByteMask(REG_002EAA, BIT4, MASKBIT(7:4)); // REG_MHL_CBUS0_55[7:4]: cbus requester transmit opportunity after arbitration
            msWrite2ByteMask(REG_002EE0, BIT14, BIT14); // REG_MHL_CBUS0_70[14]: let edid current read initial address add 1
            // For CTS 6.3.10.7
            msWrite2ByteMask(REG_002EB2, 0xADB0, MASKBIT(15:0)); // REG_MHL_CBUS0_59[15:0], timeout for a device receiving a packet within a command
            msWrite2ByteMask(REG_002EB4, 0x0001, MASKBIT(15:0)); // REG_MHL_CBUS0_5A[15:0], modify to 110ms
            // HW auto reply setting
            msWrite2ByteMask(REG_002E46, BIT10 |BIT4, MASKBIT(10:8) |BIT4); // REG_MHL_CBUS0_23[10]: enable HW auto response read_devcap request, [9:8]: old mode RCP/RAP HW auto response, [4]: enable HW responses wrt_grt_int.
            msWrite2ByteMask(REG_002EDA, MASKBIT(11:0), MASKBIT(11:0)); // REG_MHL_CBUS0_6D[11:0], Enable MHL HW mode
            msWrite2ByteMask(REG_002EE2, MASKBIT(15:14), MASKBIT(15:14)); // REG_MHL_CBUS0_71[15:14], [15]: Enable msc hw auto response., [14]:Auto response packet after SW clr fifo or Xdata.
            msWrite2ByteMask(REG_002EA0, BIT13, BIT13); // REG_MHL_CBUS0_50[13]: MSC sending command keep mode once auto response is onging
            // Xdata enable
            msWrite2ByteMask(REG_002ED8, BIT14, BIT14); // REG_MHL_CBUS0_6C[14]: Register xdata enable
            // Extend wake up pulse width
            msWrite2ByteMask(REG_002F22, 0x88B8, MASKBIT(15:0)); // extend the maxi wakeup pulse width to 35msec 
            // Free force to 1K
            msWrite2ByteMask(REG_002F02, 0, MASKBIT(5:2)); // REG_PM_MHL_CBUS0_01[5][3][2]

            //msWrite2ByteMask(REG_000462, 0, BIT1); // REG_PAD_TOP_31[1], enable pad pull-high port0 
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            // Cbus pad config
            //msWrite2ByteMask(REG_000462, 0, BIT2); // REG_PAD_TOP_31[2], enable pad pull-high port1
            msWrite2ByteMask(REG_003162, BIT11, BIT11); // REG_PM_MHL_CBUS1_31[11]
            // Set accepted discover pulse high pulse width to ignore USB pulse
            msWrite2ByteMask(REG_003118, 0x0046, MASKBIT(15:0)); // REG_PM_MHL_CBUS1_0C[15:0], reject cbus discovery pulse below this limit
            msWrite2ByteMask(REG_00311A, 0x0082, MASKBIT(15:0)); // REG_PM_MHL_CBUS1_0D[15:0], reject cbus discovery pulse above this limit
            // CTS 4.3.7.1
            msWrite2ByteMask(REG_0030AA, BIT4, MASKBIT(7:4)); // REG_MHL_CBUS1_55[7:4]: cbus requester transmit opportunity after arbitration
            msWrite2ByteMask(REG_0030E0, BIT14, BIT14); // REG_MHL_CBUS1_70[14]: let edid current read initial address add 1
            // For CTS 6.3.10.7
            msWrite2ByteMask(REG_0030B2, 0xADB0, MASKBIT(15:0)); // REG_MHL_CBUS1_59[15:0], timeout for a device receiving a packet within a command
            msWrite2ByteMask(REG_0030B4, 0x0001, MASKBIT(15:0)); // REG_MHL_CBUS1_5A[15:0], modify to 110ms
            // HW auto reply setting
            msWrite2ByteMask(REG_003046, BIT10 |BIT4, MASKBIT(10:8) |BIT4); // REG_MHL_CBUS1_23[10]: enable HW auto response read_devcap request, [9:8]: old mode RCP/RAP HW auto response, [4]: enable HW responses wrt_grt_int.
            msWrite2ByteMask(REG_0030DA, MASKBIT(11:0), MASKBIT(11:0)); // REG_MHL_CBUS1_6D[11:0], Enable MHL HW mode
            msWrite2ByteMask(REG_0030E2, MASKBIT(15:14), MASKBIT(15:14)); // REG_MHL_CBUS1_71[15:14], [15]: Enable msc hw auto response., [14]:Auto response packet after SW clr fifo or Xdata.
            msWrite2ByteMask(REG_0030A0, BIT13, BIT13); // REG_MHL_CBUS1_50[13]: MSC sending command keep mode once auto response is onging
            // Xdata enable
            msWrite2ByteMask(REG_0030D8, BIT14, BIT14); // REG_MHL_CBUS1_6C[14]: Register xdata enable
            // Extend wake up pulse width
            msWrite2ByteMask(REG_003122, 0x88B8, MASKBIT(15:0)); // extend the maxi wakeup pulse width to 35msec 
            // Free force to 1K
            msWrite2ByteMask(REG_003102, 0, MASKBIT(5:2)); // REG_PM_MHL_CBUS1_01[5][3][2]

            //msWrite2ByteMask(REG_000462, 0, BIT3); // REG_PAD_TOP_31[3], enable pad pull-high port1
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            // Cbus pad config
            //msWrite2ByteMask(REG_000462, 0, BIT4); // REG_PAD_TOP_31[4], enable pad pull-high port2
            msWrite2ByteMask(REG_003362, BIT11, BIT11); // REG_PM_MHL_CBUS2_31[11]
            // Set accepted discover pulse high pulse width to ignore USB pulse
            msWrite2ByteMask(REG_003318, 0x0046, MASKBIT(15:0)); // REG_PM_MHL_CBUS2_0C[15:0], reject cbus discovery pulse below this limit
            msWrite2ByteMask(REG_00331A, 0x0082, MASKBIT(15:0)); // REG_PM_MHL_CBUS2_0D[15:0], reject cbus discovery pulse above this limit
            // CTS 4.3.7.1
            msWrite2ByteMask(REG_0032AA, BIT4, MASKBIT(7:4)); // REG_MHL_CBUS2_55[7:4]: cbus requester transmit opportunity after arbitration
            msWrite2ByteMask(REG_0032E0, BIT14, BIT14); // REG_MHL_CBUS2_70[14]: let edid current read initial address add 1
            // For CTS 6.3.10.7
            msWrite2ByteMask(REG_0032B2, 0xADB0, MASKBIT(15:0)); // REG_MHL_CBUS2_59[15:0], timeout for a device receiving a packet within a command
            msWrite2ByteMask(REG_0032B4, 0x0001, MASKBIT(15:0)); // REG_MHL_CBUS2_5A[15:0], modify to 110ms
            // HW auto reply setting
            msWrite2ByteMask(REG_003246, BIT10 |BIT4, MASKBIT(10:8) |BIT4); // REG_MHL_CBUS2_23[10]: enable HW auto response read_devcap request, [9:8]: old mode RCP/RAP HW auto response, [4]: enable HW responses wrt_grt_int.
            msWrite2ByteMask(REG_0032DA, MASKBIT(11:0), MASKBIT(11:0)); // REG_MHL_CBUS2_6D[11:0], Enable MHL HW mode
            msWrite2ByteMask(REG_0032E2, MASKBIT(15:14), MASKBIT(15:14)); // REG_MHL_CBUS2_71[15:14], [15]: Enable msc hw auto response., [14]:Auto response packet after SW clr fifo or Xdata.
            msWrite2ByteMask(REG_0032A0, BIT13, BIT13); // REG_MHL_CBUS2_50[13]: MSC sending command keep mode once auto response is onging
            // Xdata enable
            msWrite2ByteMask(REG_0032D8, BIT14, BIT14); // REG_MHL_CBUS2_6C[14]: Register xdata enable
            // Extend wake up pulse width
            msWrite2ByteMask(REG_003322, 0x88B8, MASKBIT(15:0)); // extend the maxi wakeup pulse width to 35msec 
            // Free force to 1K
            msWrite2ByteMask(REG_003302, 0, MASKBIT(5:2)); // REG_PM_MHL_CBUS2_01[5][3][2]

            //msWrite2ByteMask(REG_000462, 0, BIT5); // REG_PAD_TOP_31[1], enable pad pull-high port2
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            // Cbus pad config
            //msWrite2ByteMask(REG_000462, 0, BIT6); // REG_PAD_TOP_31[6], enable pad pull-high port3
            msWrite2ByteMask(REG_003562, BIT11, BIT11); // REG_PM_MHL_CBUS3_31[11]
            // Set accepted discover pulse high pulse width to ignore USB pulse
            msWrite2ByteMask(REG_003518, 0x0046, MASKBIT(15:0)); // REG_PM_MHL_CBUS3_0C[15:0], reject cbus discovery pulse below this limit
            msWrite2ByteMask(REG_00351A, 0x0082, MASKBIT(15:0)); // REG_PM_MHL_CBUS3_0D[15:0], reject cbus discovery pulse above this limit
            // CTS 4.3.7.1
            msWrite2ByteMask(REG_0034AA, BIT4, MASKBIT(7:4)); // REG_MHL_CBUS3_55[7:4]: cbus requester transmit opportunity after arbitration
            msWrite2ByteMask(REG_0034E0, BIT14, BIT14); // REG_MHL_CBUS3_70[14]: let edid current read initial address add 1
            // For CTS 6.3.10.7
            msWrite2ByteMask(REG_0034B2, 0xADB0, MASKBIT(15:0)); // REG_MHL_CBUS3_59[15:0], timeout for a device receiving a packet within a command
            msWrite2ByteMask(REG_0034B4, 0x0001, MASKBIT(15:0)); // REG_MHL_CBUS3_5A[15:0], modify to 110ms
            // HW auto reply setting
            msWrite2ByteMask(REG_003446, BIT10 |BIT4, MASKBIT(10:8) |BIT4); // REG_MHL_CBUS3_23[10]: enable HW auto response read_devcap request, [9:8]: old mode RCP/RAP HW auto response, [4]: enable HW responses wrt_grt_int.
            msWrite2ByteMask(REG_0034DA, MASKBIT(11:0), MASKBIT(11:0)); // REG_MHL_CBUS3_6D[11:0], Enable MHL HW mode
            msWrite2ByteMask(REG_0034E2, MASKBIT(15:14), MASKBIT(15:14)); // REG_MHL_CBUS3_71[15:14], [15]: Enable msc hw auto response., [14]:Auto response packet after SW clr fifo or Xdata.
            msWrite2ByteMask(REG_0034A0, BIT13, BIT13); // REG_MHL_CBUS3_50[13]: MSC sending command keep mode once auto response is onging
            // Xdata enable
            msWrite2ByteMask(REG_0034D8, BIT14, BIT14); // REG_MHL_CBUS3_6C[14]: Register xdata enable
            // Extend wake up pulse width
            msWrite2ByteMask(REG_003522, 0x88B8, MASKBIT(15:0)); // extend the maxi wakeup pulse width to 35msec 
            // Free force to 1K
            msWrite2ByteMask(REG_003502, 0, MASKBIT(5:2)); // REG_PM_MHL_CBUS3_01[5][3][2]

            //msWrite2ByteMask(REG_000462, 0, BIT7); // REG_PAD_TOP_31[7], enable pad pull-high port3
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            // Cbus pad config
            //msWrite2ByteMask(REG_000462, 0, BIT8); // REG_PAD_TOP_31[8], enable pad pull-high port4
            msWrite2ByteMask(REG_003762, BIT11, BIT11); // REG_PM_MHL_CBUS4_31[11]
            // Set accepted discover pulse high pulse width to ignore USB pulse
            msWrite2ByteMask(REG_003718, 0x0046, MASKBIT(15:0)); // REG_PM_MHL_CBUS4_0C[15:0], reject cbus discovery pulse below this limit
            msWrite2ByteMask(REG_00371A, 0x0082, MASKBIT(15:0)); // REG_PM_MHL_CBUS4_0D[15:0], reject cbus discovery pulse above this limit
            // CTS 4.3.7.1
            msWrite2ByteMask(REG_0036AA, BIT4, MASKBIT(7:4)); // REG_MHL_CBUS4_55[7:4]: cbus requester transmit opportunity after arbitration
            msWrite2ByteMask(REG_0036E0, BIT14, BIT14); // REG_MHL_CBUS4_70[14]: let edid current read initial address add 1
            // For CTS 6.3.10.7
            msWrite2ByteMask(REG_0036B2, 0xADB0, MASKBIT(15:0)); // REG_MHL_CBUS4_59[15:0], timeout for a device receiving a packet within a command
            msWrite2ByteMask(REG_0036B4, 0x0001, MASKBIT(15:0)); // REG_MHL_CBUS4_5A[15:0], modify to 110ms
            // HW auto reply setting
            msWrite2ByteMask(REG_003646, BIT10 |BIT4, MASKBIT(10:8) |BIT4); // REG_MHL_CBUS4_23[10]: enable HW auto response read_devcap request, [9:8]: old mode RCP/RAP HW auto response, [4]: enable HW responses wrt_grt_int.
            msWrite2ByteMask(REG_0036DA, MASKBIT(11:0), MASKBIT(11:0)); // REG_MHL_CBUS4_6D[11:0], Enable MHL HW mode
            msWrite2ByteMask(REG_0036E2, MASKBIT(15:14), MASKBIT(15:14)); // REG_MHL_CBUS4_71[15:14], [15]: Enable msc hw auto response., [14]:Auto response packet after SW clr fifo or Xdata.
            msWrite2ByteMask(REG_0036A0, BIT13, BIT13); // REG_MHL_CBUS4_50[13]: MSC sending command keep mode once auto response is onging
            // Xdata enable
            msWrite2ByteMask(REG_0036D8, BIT14, BIT14); // REG_MHL_CBUS4_6C[14]: Register xdata enable
            // Extend wake up pulse width
            msWrite2ByteMask(REG_003722, 0x88B8, MASKBIT(15:0)); // extend the maxi wakeup pulse width to 35msec 
            // Free force to 1K
            msWrite2ByteMask(REG_003702, 0, MASKBIT(5:2)); // REG_PM_MHL_CBUS4_01[5][3][2]

            //msWrite2ByteMask(REG_000462, 0, BIT9); // REG_PAD_TOP_31[9], enable pad pull-high port4
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            // Cbus pad config
            //msWrite2ByteMask(REG_000462, 0, BIT10); // REG_PAD_TOP_31[10], enable pad pull-high port5
            msWrite2ByteMask(REG_003962, BIT11, BIT11); // REG_PM_MHL_CBUS5_31[11]
            // Set accepted discover pulse high pulse width to ignore USB pulse
            msWrite2ByteMask(REG_003918, 0x0046, MASKBIT(15:0)); // REG_PM_MHL_CBUS5_0C[15:0], reject cbus discovery pulse below this limit
            msWrite2ByteMask(REG_00391A, 0x0082, MASKBIT(15:0)); // REG_PM_MHL_CBUS5_0D[15:0], reject cbus discovery pulse above this limit
            // CTS 4.3.7.1
            msWrite2ByteMask(REG_0038AA, BIT4, MASKBIT(7:4)); // REG_MHL_CBUS5_55[7:4]: cbus requester transmit opportunity after arbitration
            msWrite2ByteMask(REG_0038E0, BIT14, BIT14); // REG_MHL_CBUS5_70[14]: let edid current read initial address add 1
            // For CTS 6.3.10.7
            msWrite2ByteMask(REG_0038B2, 0xADB0, MASKBIT(15:0)); // REG_MHL_CBUS5_59[15:0], timeout for a device receiving a packet within a command
            msWrite2ByteMask(REG_0038B4, 0x0001, MASKBIT(15:0)); // REG_MHL_CBUS5_5A[15:0], modify to 110ms
            // HW auto reply setting
            msWrite2ByteMask(REG_003846, BIT10 |BIT4, MASKBIT(10:8) |BIT4); // REG_MHL_CBUS5_23[10]: enable HW auto response read_devcap request, [9:8]: old mode RCP/RAP HW auto response, [4]: enable HW responses wrt_grt_int.
            msWrite2ByteMask(REG_0038DA, MASKBIT(11:0), MASKBIT(11:0)); // REG_MHL_CBUS5_6D[11:0], Enable MHL HW mode
            msWrite2ByteMask(REG_0038E2, MASKBIT(15:14), MASKBIT(15:14)); // REG_MHL_CBUS5_71[15:14], [15]: Enable msc hw auto response., [14]:Auto response packet after SW clr fifo or Xdata.
            msWrite2ByteMask(REG_0038A0, BIT13, BIT13); // REG_MHL_CBUS5_50[13]: MSC sending command keep mode once auto response is onging
            // Xdata enable
            msWrite2ByteMask(REG_0038D8, BIT14, BIT14); // REG_MHL_CBUS5_6C[14]: Register xdata enable
            // Extend wake up pulse width
            msWrite2ByteMask(REG_003922, 0x88B8, MASKBIT(15:0)); // extend the maxi wakeup pulse width to 35msec 
            // Free force to 1K
            msWrite2ByteMask(REG_003902, 0, MASKBIT(5:2)); // REG_PM_MHL_CBUS5_01[5][3][2]

            //msWrite2ByteMask(REG_000462, 0, BIT11); // REG_PAD_TOP_31[11], enable pad pull-high port5
            
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_LoadDevcapVenderID()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_LoadDevcapVenderID(BYTE ucPortSelect, BYTE *pDevcap, BYTE ucVenderID)
{
    BYTE ucIndex = 0;
    BYTE uctemp = 0; 

    MHL_HAL_DPRINTF("** MHL load DevCap... port %d", ucPortSelect); 

    if(pDevcap != NULL)
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                // Load vendor ID
                msWriteByte(REG_002E01, ucVenderID); // REG_MHL_CBUS0_00[15:8], MHL_VENDOR_ID

                // Load device capability
                for(uctemp = 0; uctemp <8; uctemp++)
                {
                    if(uctemp == 2)
                    {
                        ucIndex = 1;
                    }
                    
                    msWrite2Byte(REG_002E02 +(uctemp *2), (pDevcap[(uctemp *2) +1 -ucIndex] <<8) | pDevcap[uctemp *2 -ucIndex]);
                }
                
                msWrite2Byte(REG_002E06, (pDevcap[3] <<8) | pDevcap[4]); // REG_MHL_CBUS0_03[15:0], MHL_ADOPTER_ID
                msWrite2Byte(REG_002E0E, (pDevcap[11] <<8) | pDevcap[12]); // REG_MHL_CBUS0_07[15:0], MHL_DEVICE_ID
                msWriteByte(REG_002E12, pDevcap[15]); // REG_MHL_CBUS0_09[7:0], reg_devcap_0f
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                // Load vendor ID
                msWriteByte(REG_003001, ucVenderID); // REG_MHL_CBUS1_00[15:8], MHL_VENDOR_ID

                // Load device capability
                for(uctemp = 0; uctemp <8; uctemp++)
                {
                    if(uctemp == 2)
                    {
                        ucIndex = 1;
                    }
                    
                    msWrite2Byte(REG_003002 +(uctemp *2), (pDevcap[(uctemp *2) +1 -ucIndex] <<8) | pDevcap[uctemp *2 -ucIndex]);
                }
                
                msWrite2Byte(REG_003006, (pDevcap[3] <<8) | pDevcap[4]); // REG_MHL_CBUS1_03[15:0], MHL_ADOPTER_ID
                msWrite2Byte(REG_00300E, (pDevcap[11] <<8) | pDevcap[12]); // REG_MHL_CBUS1_07[15:0], MHL_DEVICE_ID
                msWriteByte(REG_003012, pDevcap[15]); // REG_MHL_CBUS1_09[7:0], reg_devcap_0f
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
            case MHL_INPUT_SELECT_PORT2:
                // Load vendor ID
                msWriteByte(REG_003201, ucVenderID); // REG_MHL_CBUS2_00[15:8], MHL_VENDOR_ID

                // Load device capability
                for(uctemp = 0; uctemp <8; uctemp++)
                {
                    if(uctemp == 2)
                    {
                        ucIndex = 1;
                    }
                    
                    msWrite2Byte(REG_003202 +(uctemp *2), (pDevcap[(uctemp *2) +1 -ucIndex] <<8) | pDevcap[uctemp *2 -ucIndex]);
                }
                
                msWrite2Byte(REG_003206, (pDevcap[3] <<8) | pDevcap[4]); // REG_MHL_CBUS2_03[15:0], MHL_ADOPTER_ID
                msWrite2Byte(REG_00320E, (pDevcap[11] <<8) | pDevcap[12]); // REG_MHL_CBUS2_07[15:0], MHL_DEVICE_ID
                msWriteByte(REG_003212, pDevcap[15]); // REG_MHL_CBUS2_09[7:0], reg_devcap_0f
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
            case MHL_INPUT_SELECT_PORT3:
                // Load vendor ID
                msWriteByte(REG_003401, ucVenderID); // REG_MHL_CBUS3_00[15:8], MHL_VENDOR_ID

                // Load device capability
                for(uctemp = 0; uctemp <8; uctemp++)
                {
                    if(uctemp == 2)
                    {
                        ucIndex = 1;
                    }
                    
                    msWrite2Byte(REG_003402 +(uctemp *2), (pDevcap[(uctemp *2) +1 -ucIndex] <<8) | pDevcap[uctemp *2 -ucIndex]);
                }
                
                msWrite2Byte(REG_003406, (pDevcap[3] <<8) | pDevcap[4]); // REG_MHL_CBUS3_03[15:0], MHL_ADOPTER_ID
                msWrite2Byte(REG_00340E, (pDevcap[11] <<8) | pDevcap[12]); // REG_MHL_CBUS3_07[15:0], MHL_DEVICE_ID
                msWriteByte(REG_003412, pDevcap[15]); // REG_MHL_CBUS3_09[7:0], reg_devcap_0f
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
            case MHL_INPUT_SELECT_PORT4:
                // Load vendor ID
                msWriteByte(REG_003601, ucVenderID); // REG_MHL_CBUS4_00[15:8], MHL_VENDOR_ID

                // Load device capability
                for(uctemp = 0; uctemp <8; uctemp++)
                {
                    if(uctemp == 2)
                    {
                        ucIndex = 1;
                    }
                    
                    msWrite2Byte(REG_003602 +(uctemp *2), (pDevcap[(uctemp *2) +1 -ucIndex] <<8) | pDevcap[uctemp *2 -ucIndex]);
                }
                
                msWrite2Byte(REG_003606, (pDevcap[3] <<8) | pDevcap[4]); // REG_MHL_CBUS4_03[15:0], MHL_ADOPTER_ID
                msWrite2Byte(REG_00360E, (pDevcap[11] <<8) | pDevcap[12]); // REG_MHL_CBUS4_07[15:0], MHL_DEVICE_ID
                msWriteByte(REG_003612, pDevcap[15]); // REG_MHL_CBUS4_09[7:0], reg_devcap_0f
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
            case MHL_INPUT_SELECT_PORT5:
                // Load vendor ID
                msWriteByte(REG_003801, ucVenderID); // REG_MHL_CBUS5_00[15:8], MHL_VENDOR_ID

                // Load device capability
                for(uctemp = 0; uctemp <8; uctemp++)
                {
                    if(uctemp == 2)
                    {
                        ucIndex = 1;
                    }
                    
                    msWrite2Byte(REG_003802 +(uctemp *2), (pDevcap[(uctemp *2) +1 -ucIndex] <<8) | pDevcap[uctemp *2 -ucIndex]);
                }
                
                msWrite2Byte(REG_003806, (pDevcap[3] <<8) | pDevcap[4]); // REG_MHL_CBUS5_03[15:0], MHL_ADOPTER_ID
                msWrite2Byte(REG_00380E, (pDevcap[11] <<8) | pDevcap[12]); // REG_MHL_CBUS5_07[15:0], MHL_DEVICE_ID
                msWriteByte(REG_003812, pDevcap[15]); // REG_MHL_CBUS5_09[7:0], reg_devcap_0f
                
                break;
#endif

            default:

                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ComboPhySetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_ComboPhySetting(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWrite2ByteMask(REG_11209E, MHL_24BIT_VCO_THRESHOLD1, MASKBIT(15:0)); // REG_COMBO_PHY0_4F[15:0], 24bit mode clock threshold1
            msWrite2ByteMask(REG_11209C, MHL_PPMODE_VCO_THRESHOLD1, MASKBIT(15:0)); // REG_COMBO_PHY0_4E[15:0], PP mode clock threshold1
            msWrite2ByteMask(REG_11209A, MHL_PPMODE_VCO_THRESHOLD2, MASKBIT(15:0)); // REG_COMBO_PHY0_4D[15:0], PP mode clock threshold2
            msWrite2ByteMask(REG_160504, BIT3, BIT3); // REG_COMBO_DECODER0_02[3]: port0 swap for MHL v1.2
            msWrite2ByteMask(REG_1120DE, BIT14, BIT14); // REG_COMBO_PHY0_6F[14], port0 i-control PD overwrite
            msWrite2ByteMask(REG_1120D8, (MHL_I_CONTROL_PD_VALUE << 9), MASKBIT(13:9)); // REG_COMBO_PHY0_6C[13:9], port0 i-control PD overwrite value
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWrite2ByteMask(REG_11219E, MHL_24BIT_VCO_THRESHOLD1, MASKBIT(15:0)); // REG_COMBO_PHY1_4F[15:0], 24bit mode clock threshold
            msWrite2ByteMask(REG_11219C, MHL_PPMODE_VCO_THRESHOLD1, MASKBIT(15:0)); // REG_COMBO_PHY1_4E[15:0], PP mode clock threshold1
            msWrite2ByteMask(REG_11219A, MHL_PPMODE_VCO_THRESHOLD2, MASKBIT(15:0)); // REG_COMBO_PHY1_4D[15:0], PP mode clock threshold2
            msWrite2ByteMask(REG_160B04, BIT3, BIT3); // REG_COMBO_DECODER1_02[3]: port1 swap for MHL v1.2
            msWrite2ByteMask(REG_1121DE, BIT14, BIT14); // REG_COMBO_PHY1_6F[14], port1 i-control PD overwrite
            msWrite2ByteMask(REG_1121D8, (MHL_I_CONTROL_PD_VALUE << 9), MASKBIT(13:9)); // REG_COMBO_PHY1_6C[13:9], port1 i-control PD overwrite value
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            msWrite2ByteMask(REG_11229E, MHL_24BIT_VCO_THRESHOLD1, MASKBIT(15:0)); // REG_COMBO_PHY2_4F[15:0], 24bit mode clock threshold
            msWrite2ByteMask(REG_11229C, MHL_PPMODE_VCO_THRESHOLD1, MASKBIT(15:0)); // REG_COMBO_PHY2_4E[15:0], PP mode clock threshold1
            msWrite2ByteMask(REG_11229A, MHL_PPMODE_VCO_THRESHOLD2, MASKBIT(15:0)); // REG_COMBO_PHY2_4D[15:0], PP mode clock threshold2
            msWrite2ByteMask(REG_161304, BIT3, BIT3); // REG_COMBO_DECODER2_02[3]: port2 swap for MHL v1.2
            msWrite2ByteMask(REG_1122DE, BIT14, BIT14); // REG_COMBO_PHY2_6F[14], port2 i-control PD overwrite
            msWrite2ByteMask(REG_1122D8, (MHL_I_CONTROL_PD_VALUE << 9), MASKBIT(13:9)); // REG_COMBO_PHY2_6C[13:9], port2 i-control PD overwrite value
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            msWrite2ByteMask(REG_11239E, MHL_24BIT_VCO_THRESHOLD1, MASKBIT(15:0)); // REG_COMBO_PHY3_4F[15:0], 24bit mode clock threshold
            msWrite2ByteMask(REG_11239C, MHL_PPMODE_VCO_THRESHOLD1, MASKBIT(15:0)); // REG_COMBO_PHY3_4E[15:0], PP mode clock threshold1
            msWrite2ByteMask(REG_11239A, MHL_PPMODE_VCO_THRESHOLD2, MASKBIT(15:0)); // REG_COMBO_PHY3_4D[15:0], PP mode clock threshold2
            msWrite2ByteMask(REG_161B04, BIT3, BIT3); // REG_COMBO_DECODER3_02[3]: port3 swap for MHL v1.2
            msWrite2ByteMask(REG_1123DE, BIT14, BIT14); // REG_COMBO_PHY3_6F[14], port3 i-control PD overwrite
            msWrite2ByteMask(REG_1123D8, (MHL_I_CONTROL_PD_VALUE << 9), MASKBIT(13:9)); // REG_COMBO_PHY3_6C[13:9], port3 i-control PD overwrite value
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            msWrite2ByteMask(REG_11249E, MHL_24BIT_VCO_THRESHOLD1, MASKBIT(15:0)); // REG_COMBO_PHY4_4F[15:0], 24bit mode clock threshold
            msWrite2ByteMask(REG_11249C, MHL_PPMODE_VCO_THRESHOLD1, MASKBIT(15:0)); // REG_COMBO_PHY4_4E[15:0], PP mode clock threshold1
            msWrite2ByteMask(REG_11249A, MHL_PPMODE_VCO_THRESHOLD2, MASKBIT(15:0)); // REG_COMBO_PHY4_4D[15:0], PP mode clock threshold2
            msWrite2ByteMask(REG_162304, BIT3, BIT3); // REG_COMBO_DECODER4_02[3]: port4 swap for MHL v1.2
            msWrite2ByteMask(REG_1124DE, BIT14, BIT14); // REG_COMBO_PHY4_6F[14], port4 i-control PD overwrite
            msWrite2ByteMask(REG_1124D8, (MHL_I_CONTROL_PD_VALUE << 9), MASKBIT(13:9)); // REG_COMBO_PHY4_6C[13:9], port4 i-control PD overwrite value
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            msWrite2ByteMask(REG_11259E, MHL_24BIT_VCO_THRESHOLD1, MASKBIT(15:0)); // REG_COMBO_PHY5_4F[15:0], 24bit mode clock threshold
            msWrite2ByteMask(REG_11259C, MHL_PPMODE_VCO_THRESHOLD1, MASKBIT(15:0)); // REG_COMBO_PHY5_4E[15:0], PP mode clock threshold1
            msWrite2ByteMask(REG_11259A, MHL_PPMODE_VCO_THRESHOLD2, MASKBIT(15:0)); // REG_COMBO_PHY5_4D[15:0], PP mode clock threshold2
            msWrite2ByteMask(REG_162B04, BIT3, BIT3); // REG_COMBO_DECODER5_02[3]: port5 swap for MHL v1.2
            msWrite2ByteMask(REG_1125DE, BIT14, BIT14); // REG_COMBO_PHY5_6F[14], port5 i-control PD overwrite
            msWrite2ByteMask(REG_1125D8, (MHL_I_CONTROL_PD_VALUE << 9), MASKBIT(13:9)); // REG_COMBO_PHY5_6C[13:9], port5 i-control PD overwrite value
            
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetCableDetect()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetCableDetect(BYTE ucPortSelect, Bool bFlag)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bFlag)
            {
                msWriteByteMask(REG_002F01, MASKBIT(1:0), MASKBIT(1:0)); // REG_PM_MHL_CBUS0_00[9:8], SW control cable detect
            }
            else
            {
                msWriteByteMask(REG_002F01, BIT1, MASKBIT(1:0)); // REG_PM_MHL_CBUS0_00[9:8], SW control cable detect
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bFlag)
            {
                msWriteByteMask(REG_003101, MASKBIT(1:0), MASKBIT(1:0)); // REG_PM_MHL_CBUS1_00[9:8], SW control cable detect
            }
            else
            {
                msWriteByteMask(REG_003101, BIT1, MASKBIT(1:0)); // REG_PM_MHL_CBUS1_00[9:8], SW control cable detect
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(bFlag)
            {
                msWriteByteMask(REG_003301, MASKBIT(1:0), MASKBIT(1:0)); // REG_PM_MHL_CBUS2_00[9:8], SW control cable detect
            }
            else
            {
                msWriteByteMask(REG_003301, BIT1, MASKBIT(1:0)); // REG_PM_MHL_CBUS2_00[9:8], SW control cable detect
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(bFlag)
            {
                msWriteByteMask(REG_003501, MASKBIT(1:0), MASKBIT(1:0)); // REG_PM_MHL_CBUS3_00[9:8], SW control cable detect
            }
            else
            {
                msWriteByteMask(REG_003501, BIT1, MASKBIT(1:0)); // REG_PM_MHL_CBUS3_00[9:8], SW control cable detect
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(bFlag)
            {
                msWriteByteMask(REG_003701, MASKBIT(1:0), MASKBIT(1:0)); // REG_PM_MHL_CBUS4_00[9:8], SW control cable detect
            }
            else
            {
                msWriteByteMask(REG_003701, BIT1, MASKBIT(1:0)); // REG_PM_MHL_CBUS4_00[9:8], SW control cable detect
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(bFlag)
            {
                msWriteByteMask(REG_003901, MASKBIT(1:0), MASKBIT(1:0)); // REG_PM_MHL_CBUS5_00[9:8], SW control cable detect
            }
            else
            {
                msWriteByteMask(REG_003901, BIT1, MASKBIT(1:0)); // REG_PM_MHL_CBUS5_00[9:8], SW control cable detect
            }
            
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CbusPadEnable()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_CbusPadEnable(BYTE ucPortSelect, Bool bEnableCbus)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            //msWrite2ByteMask(REG_000460, bEnableCbus? BIT0: 0, BIT0); // REG_PAD_TOP_30[0], Cbus port0 enable
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            //msWrite2ByteMask(REG_000460, bEnableCbus? BIT1: 0, BIT1); // REG_PAD_TOP_30[1], Cbus port1 enable
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            //msWrite2ByteMask(REG_000460, bEnableCbus? BIT2: 0, BIT2); // REG_PAD_TOP_30[2], Cbus port2 enable
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            //msWrite2ByteMask(REG_000460, bEnableCbus? BIT3: 0, BIT3); // REG_PAD_TOP_30[3], Cbus port3 enable
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            //msWrite2ByteMask(REG_000460, bEnableCbus? BIT4: 0, BIT4); // REG_PAD_TOP_30[4], Cbus port4 enable
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            //msWrite2ByteMask(REG_000460, bEnableCbus? BIT5: 0, BIT5); // REG_PAD_TOP_30[5], Cbus port5 enable
            break;
#endif

        default:
            bEnableCbus = 0;
            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetVbusCharge()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetVbusCharge(BYTE ucPortSelect, Bool bFlag)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bFlag)
            {
                SET_MHL_VBUS_CHARGE_PORT0();
            }
            else
            {
                CLR_MHL_VBUS_CHARGE_PORT0();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bFlag)
            {
                SET_MHL_VBUS_CHARGE_PORT1();
            }
            else
            {
                CLR_MHL_VBUS_CHARGE_PORT1();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(bFlag)
            {
                SET_MHL_VBUS_CHARGE_PORT2();
            }
            else
            {
                CLR_MHL_VBUS_CHARGE_PORT2();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(bFlag)
            {
                SET_MHL_VBUS_CHARGE_PORT3();
            }
            else
            {
                CLR_MHL_VBUS_CHARGE_PORT3();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(bFlag)
            {
                SET_MHL_VBUS_CHARGE_PORT4();
            }
            else
            {
                CLR_MHL_VBUS_CHARGE_PORT4();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(bFlag)
            {
                SET_MHL_VBUS_CHARGE_PORT5();
            }
            else
            {
                CLR_MHL_VBUS_CHARGE_PORT5();
            }
            
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetHPDControl()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetHPDControl(BYTE ucPortSelect, Bool bSetFlag)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            if(bSetFlag)
            {
                SET_MHL_HDMI_HPD_PORT0();
            }
            else
            {
                CLR_MHL_HDMI_HPD_PORT0();
            }
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            if(bSetFlag)
            {
                SET_MHL_HDMI_HPD_PORT1();
            }
            else
            {
                CLR_MHL_HDMI_HPD_PORT1();
            }
            
            break;

        case MHL_INPUT_SELECT_PORT2:
            if(bSetFlag)
            {
                SET_MHL_HDMI_HPD_PORT2();
            }
            else
            {
                CLR_MHL_HDMI_HPD_PORT2();
            }
            
            break;

        case MHL_INPUT_SELECT_PORT3:
            if(bSetFlag)
            {
                SET_MHL_HDMI_HPD_PORT3();
            }
            else
            {
                CLR_MHL_HDMI_HPD_PORT3();
            }
            
            break;

        case MHL_INPUT_SELECT_PORT4:
            if(bSetFlag)
            {
                SET_MHL_HDMI_HPD_PORT4();
            }
            else
            {
                CLR_MHL_HDMI_HPD_PORT4();
            }
            
            break;

        case MHL_INPUT_SELECT_PORT5:
            if(bSetFlag)
            {
                SET_MHL_HDMI_HPD_PORT5();
            }
            else
            {
                CLR_MHL_HDMI_HPD_PORT5();
            }
            
            break;
            
        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_OpenCommonModeResistor()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_OpenCommonModeResistor(BYTE ucPortSelect, Bool bFlag)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWrite2ByteMask(REG_0017A4, bFlag? 0: BIT8, BIT8); // REG_PM_BANK_52[8], overwrite port0 enable mhl mode in sw mode
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWrite2ByteMask(REG_0017A4, bFlag? 0: BIT9, BIT9); // REG_PM_BANK_52[9], overwrite port1 enable mhl mode in sw mode
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            msWrite2ByteMask(REG_0017A4, bFlag? 0: BIT10, BIT10); // REG_PM_BANK_52[10], overwrite port2 enable mhl mode in sw mode
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            msWrite2ByteMask(REG_0017A4, bFlag? 0: BIT11, BIT11); // REG_PM_BANK_52[11], overwrite port3 enable mhl mode in sw mode
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            msWrite2ByteMask(REG_0017A4, bFlag? 0: BIT12, BIT12); // REG_PM_BANK_52[12], overwrite port4 enable mhl mode in sw mode
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            msWrite2ByteMask(REG_0017A4, bFlag? 0: BIT13, BIT13); // REG_PM_BANK_52[13], overwrite port5 enable mhl mode in sw mode
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_HDMIByPassSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_HDMIByPassSetting(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            //msWrite2ByteMask(REG_0017A4, 0, BIT0 |BIT8); // REG_PM_BANK_52[0][8], overwrite port0 enable mhl mode, enable mhl mode in sw mode
            msWrite2ByteMask(REG_1120A6, 0, MASKBIT(3:0)); // REG_COMBO_PHY0_53[3:0], port0 phase overwrite enable
            msWrite2ByteMask(REG_1120B2, 0, BIT8); // REG_COMBO_PHY0_59[8], port0 rterm control overwrite
            msWrite2ByteMask(REG_1120B6, 0, MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY0_5B[15:12][1], port0 power down lane, rterm control
            msWrite2ByteMask(REG_1120DC, MASKBIT(9:6), MASKBIT(9:6)); // REG_COMBO_PHY0_6E[9:6], port0 EQ overwrite
            msWrite2Byte(REG_1120E6, ((MHL_HDMI14_R_CHANNEL_EQ << 10) |(MHL_HDMI14_G_CHANNEL_EQ << 5) |MHL_HDMI14_B_CHANNEL_EQ)); // REG_COMBO_PHY0_73[4:0], port0 EQ overwrite value
            msWrite2ByteMask(REG_160504, 0, BIT1); // REG_COMBO_DECODER0_02[1]: port0 packed-pixel mode enable
            msWrite2ByteMask(REG_16043C, 0, BIT2); // REG_COMBO_HDCP0_1E[2]: port0 packed-pixel mode + HDCP enable
            //msWriteByteMask(REG_1120DF, 0, BIT7); // REG_COMBO_PHY0_6F[15], port0 i-control PFD overwrite
            msWriteByteMask(REG_1120B9, 0, BIT1); // REG_COMBO_PHY0_5C[9], port0 enable comparator 
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            //msWrite2ByteMask(REG_0017A4, 0, BIT1 |BIT9); // REG_PM_BANK_52[1][9], overwrite port1 enable mhl mode, enable mhl mode in sw mode
            msWrite2ByteMask(REG_1121A6, 0, MASKBIT(3:0)); // REG_COMBO_PHY1_53[3:0], port1 phase overwrite enable
            msWrite2ByteMask(REG_1121B2, 0, BIT8); // REG_COMBO_PHY1_59[8], port1 rterm control overwrite
            msWrite2ByteMask(REG_1121B6, 0, MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY1_5B[15:12][1], port1 power down lane, rterm control
            msWrite2ByteMask(REG_1121DC, MASKBIT(9:6), MASKBIT(9:6)); // REG_COMBO_PHY1_6E[9:6], port1 EQ overwrite
            msWrite2Byte(REG_1121E6, ((MHL_HDMI14_R_CHANNEL_EQ << 10) |(MHL_HDMI14_G_CHANNEL_EQ << 5) |MHL_HDMI14_B_CHANNEL_EQ)); // REG_COMBO_PHY1_73[4:0], port1 EQ overwrite value
            msWrite2ByteMask(REG_160B04, 0, BIT1); // REG_COMBO_DECODER1_02[1]: port1 packed-pixel mode enable
            msWrite2ByteMask(REG_160A3C, 0, BIT2); // REG_COMBO_HDCP1_1E[2]: port1 packed-pixel mode + HDCP enable
            //msWriteByteMask(REG_1121DF, 0, BIT7); // REG_COMBO_PHY1_6F[15], port1 i-control PFD overwrite
            msWriteByteMask(REG_1121B9, 0, BIT1); // REG_COMBO_PHY1_5C[9], port1 enable comparator 

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            //msWrite2ByteMask(REG_0017A4, 0, BIT2 |BIT10); // REG_PM_BANK_52[2][10], overwrite port2 enable mhl mode, enable mhl mode in sw mode
            msWrite2ByteMask(REG_1122A6, 0, MASKBIT(3:0)); // REG_COMBO_PHY2_53[3:0], port2 phase overwrite enable
            msWrite2ByteMask(REG_1122B2, 0, BIT8); // REG_COMBO_PHY2_59[8], port2 rterm control overwrite
            msWrite2ByteMask(REG_1122B6, 0, MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY2_5B[15:12][1], port2 power down lane, rterm control
            msWrite2ByteMask(REG_1122DC, MASKBIT(9:6), MASKBIT(9:6)); // REG_COMBO_PHY2_6E[9:6], port2 EQ overwrite
            msWrite2Byte(REG_1122E6, ((MHL_HDMI14_R_CHANNEL_EQ << 10) |(MHL_HDMI14_G_CHANNEL_EQ << 5) |MHL_HDMI14_B_CHANNEL_EQ)); // REG_COMBO_PHY2_73[4:0], port2 EQ overwrite value
            msWrite2ByteMask(REG_161304, 0, BIT1); // REG_COMBO_DECODER2_02[1]: port2 packed-pixel mode enable
            msWrite2ByteMask(REG_16123C, 0, BIT2); // REG_COMBO_HDCP2_1E[2]: port2 packed-pixel mode + HDCP enable
            //msWriteByteMask(REG_1122DF, 0, BIT7); // REG_COMBO_PHY2_6F[15], port2 i-control PFD overwrite
            msWriteByteMask(REG_1122B9, 0, BIT1); // REG_COMBO_PHY2_5C[9], port2 enable comparator 

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            //msWrite2ByteMask(REG_0017A4, 0, BIT3 |BIT11); // REG_PM_BANK_52[3][11], overwrite port3 enable mhl mode, enable mhl mode in sw mode
            msWrite2ByteMask(REG_1123A6, 0, MASKBIT(3:0)); // REG_COMBO_PHY3_53[3:0], port3 phase overwrite enable
            msWrite2ByteMask(REG_1123B2, 0, BIT8); // REG_COMBO_PHY3_59[8], port3 rterm control overwrite
            msWrite2ByteMask(REG_1123B6, 0, MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY3_5B[15:12][1], port3 power down lane, rterm control
            msWrite2ByteMask(REG_1123DC, MASKBIT(9:6), MASKBIT(9:6)); // REG_COMBO_PHY3_6E[9:6], port3 EQ overwrite
            msWrite2Byte(REG_1123E6, ((MHL_HDMI14_R_CHANNEL_EQ << 10) |(MHL_HDMI14_G_CHANNEL_EQ << 5) |MHL_HDMI14_B_CHANNEL_EQ)); // REG_COMBO_PHY3_73[4:0], port3 EQ overwrite value
            msWrite2ByteMask(REG_161B04, 0, BIT1); // REG_COMBO_DECODER3_02[1]: port3 packed-pixel mode enable
            msWrite2ByteMask(REG_161A3C, 0, BIT2); // REG_COMBO_HDCP3_1E[2]: port3 packed-pixel mode + HDCP enable
            //msWriteByteMask(REG_1123DF, 0, BIT7); // REG_COMBO_PHY3_6F[15], port3 i-control PFD overwrite
            msWriteByteMask(REG_1123B9, 0, BIT1); // REG_COMBO_PHY3_5C[9], port3 enable comparator 

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            //msWrite2ByteMask(REG_0017A4, 0, BIT4 |BIT12); // REG_PM_BANK_52[4][12], overwrite port4 enable mhl mode, enable mhl mode in sw mode
            msWrite2ByteMask(REG_1124A6, 0, MASKBIT(3:0)); // REG_COMBO_PHY4_53[3:0], port4 phase overwrite enable
            msWrite2ByteMask(REG_1124B2, 0, BIT8); // REG_COMBO_PHY4_59[8], port4 rterm control overwrite
            msWrite2ByteMask(REG_1124B6, 0, MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY4_5B[15:12][1], port4 power down lane, rterm control
            msWrite2ByteMask(REG_1124DC, MASKBIT(9:6), MASKBIT(9:6)); // REG_COMBO_PHY4_6E[9:6], port4 EQ overwrite
            msWrite2Byte(REG_1124E6, ((MHL_HDMI14_R_CHANNEL_EQ << 10) |(MHL_HDMI14_G_CHANNEL_EQ << 5) |MHL_HDMI14_B_CHANNEL_EQ)); // REG_COMBO_PHY4_73[4:0], port4 EQ overwrite value
            msWrite2ByteMask(REG_162304, 0, BIT1); // REG_COMBO_DECODER4_02[1]: port4 packed-pixel mode enable
            msWrite2ByteMask(REG_16223C, 0, BIT2); // REG_COMBO_HDCP4_1E[2]: port4 packed-pixel mode + HDCP enable
            //msWriteByteMask(REG_1124DF, 0, BIT7); // REG_COMBO_PHY4_6F[15], port4 i-control PFD overwrite
            msWriteByteMask(REG_1124B9, 0, BIT1); // REG_COMBO_PHY4_5C[9], port4 enable comparator 

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            //msWrite2ByteMask(REG_0017A4, 0, BIT5 |BIT13); // REG_PM_BANK_52[5][13], overwrite port5 enable mhl mode, enable mhl mode in sw mode
            msWrite2ByteMask(REG_1125A6, 0, MASKBIT(3:0)); // REG_COMBO_PHY5_53[3:0], port5 phase overwrite enable
            msWrite2ByteMask(REG_1125B2, 0, BIT8); // REG_COMBO_PHY5_59[8], port5 rterm control overwrite
            msWrite2ByteMask(REG_1125B6, 0, MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY5_5B[15:12][1], port5 power down lane, rterm control
            msWrite2ByteMask(REG_1125DC, MASKBIT(9:6), MASKBIT(9:6)); // REG_COMBO_PHY5_6E[9:6], port5 EQ overwrite
            msWrite2Byte(REG_1125E6, ((MHL_HDMI14_R_CHANNEL_EQ << 10) |(MHL_HDMI14_G_CHANNEL_EQ << 5) |MHL_HDMI14_B_CHANNEL_EQ)); // REG_COMBO_PHY5_73[4:0], port5 EQ overwrite value
            msWrite2ByteMask(REG_162B04, 0, BIT1); // REG_COMBO_DECODER5_02[1]: port5 packed-pixel mode enable
            msWrite2ByteMask(REG_162A3C, 0, BIT2); // REG_COMBO_HDCP5_1E[2]: port5 packed-pixel mode + HDCP enable
            //msWriteByteMask(REG_1125DF, 0, BIT7); // REG_COMBO_PHY5_6F[15], port5 i-control PFD overwrite
            msWriteByteMask(REG_1125B9, 0, BIT1); // REG_COMBO_PHY5_5C[9], port5 enable comparator 

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_24BitModeSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_24BitModeSetting(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            //msWrite2ByteMask(REG_0017A4, BIT0 |BIT8, BIT0 |BIT8); // REG_PM_BANK_52[0][8], overwrite port0 enable mhl mode, enable mhl mode in sw mode
            msWrite2ByteMask(REG_1120A6, MASKBIT(3:0), MASKBIT(3:0)); // REG_COMBO_PHY0_53[3:0], port0 phase overwrite enable
            msWrite2ByteMask(REG_1120B2, BIT8, BIT8); // REG_COMBO_PHY0_59[8], port0 rterm control overwrite

#if(MHL_RCONTROL_OVERWRITE_ENABLE0)
            if((msReadByte(112000) & BIT5) == BIT5) // REG_COMBO_PHY0_00[5]: RB swap
            {
                msWrite2ByteMask(REG_1120B6, MHL_RTERM_CONTROL_VALUE0 |MASKBIT(2:1), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY0_5B[15:12][3], port0 power down lane, rterm control
            }
            else
            {
                msWrite2ByteMask(REG_1120B6, MHL_RTERM_CONTROL_VALUE0 |MASKBIT(3:2), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY0_5B[15:12][1], port0 power down lane, rterm control
            }
            
#else
            if((msReadByte(112000) & BIT5) == BIT5) // REG_COMBO_PHY0_00[5]: RB swap
            {
                msWrite2ByteMask(REG_1120B6, MHL_RTERM_CONTROL_VALUE |MASKBIT(2:1), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY0_5B[15:12][3], port0 power down lane, rterm control
            }
            else
            {
                msWrite2ByteMask(REG_1120B6, MHL_RTERM_CONTROL_VALUE |MASKBIT(3:2), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY0_5B[15:12][1], port0 power down lane, rterm control
            }
            
#endif

            msWrite2ByteMask(REG_1120DC, MASKBIT(9:6), MASKBIT(9:6)); // REG_COMBO_PHY0_6E[9:6], port0 EQ overwrite
            msWrite2ByteMask(REG_1120E6, MHL_B_CHANNEL_EQ_VALUE, MASKBIT(4:0)); // REG_COMBO_PHY0_73[4:0], port0 EQ overwrite value
            msWrite2ByteMask(REG_160504, 0, BIT1); // REG_COMBO_DECODER0_02[1]: port0 packed-pixel mode enable
            msWrite2ByteMask(REG_16043C, 0, BIT2); // REG_COMBO_HDCP0_1E[2]: port0 packed-pixel mode + HDCP enable
            //msWriteByteMask(REG_1120DF, BIT7, BIT7); // REG_COMBO_PHY0_6F[15], port0 i-control PFD overwrite
            //msWriteByteMask(REG_1120B6, (MHL_I_CONTROL_PFD_VALUE << 4), MASKBIT(7:4)); // REG_COMBO_PHY0_5B[7:4], port0 i-control PFD overwrite value
            msWriteByteMask(REG_1120B9, BIT1, BIT1); // REG_COMBO_PHY0_5C[9], port0 enable comparator 
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            //msWrite2ByteMask(REG_0017A4, BIT1 |BIT9, BIT1 |BIT9); // REG_PM_BANK_52[1][9], overwrite port1 enable mhl mode, enable mhl mode in sw mode
            msWrite2ByteMask(REG_1121A6, MASKBIT(3:0), MASKBIT(3:0)); // REG_COMBO_PHY1_53[3:0], port1 phase overwrite enable
            msWrite2ByteMask(REG_1121B2, BIT8, BIT8); // REG_COMBO_PHY1_59[8], port1 rterm control overwrite

#if(MHL_RCONTROL_OVERWRITE_ENABLE1)
            if((msReadByte(112100) & BIT5) == BIT5) // REG_COMBO_PHY1_00[5]: RB swap
            {
                msWrite2ByteMask(REG_1121B6, MHL_RTERM_CONTROL_VALUE1 |MASKBIT(2:1), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY1_5B[15:12][3], port1 power down lane, rterm control
            }
            else
            {
                msWrite2ByteMask(REG_1121B6, MHL_RTERM_CONTROL_VALUE1 |MASKBIT(3:2), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY1_5B[15:12][1], port1 power down lane, rterm control
            }
            
#else
            if((msReadByte(112100) & BIT5) == BIT5) // REG_COMBO_PHY1_00[5]: RB swap
            {
                msWrite2ByteMask(REG_1121B6, MHL_RTERM_CONTROL_VALUE |MASKBIT(2:1), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY1_5B[15:12][3], port1 power down lane, rterm control
            }
            else
            {
                msWrite2ByteMask(REG_1121B6, MHL_RTERM_CONTROL_VALUE |MASKBIT(3:2), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY1_5B[15:12][1], port1 power down lane, rterm control
            }
            
#endif

            msWrite2ByteMask(REG_1121DC, MASKBIT(9:6), MASKBIT(9:6)); // REG_COMBO_PHY1_6E[9:6], port1 EQ overwrite
            msWrite2ByteMask(REG_1121E6, MHL_B_CHANNEL_EQ_VALUE, MASKBIT(4:0)); // REG_COMBO_PHY1_73[4:0], port1 EQ overwrite value
            msWrite2ByteMask(REG_160B04, 0, BIT1); // REG_COMBO_DECODER1_02[1]: port1 packed-pixel mode enable
            msWrite2ByteMask(REG_160A3C, 0, BIT2); // REG_COMBO_HDCP1_1E[2]: port1 packed-pixel mode + HDCP enable
            //msWriteByteMask(REG_1121DF, BIT7, BIT7); // REG_COMBO_PHY1_6F[15], port1 i-control PFD overwrite
            //msWriteByteMask(REG_1121B6, (MHL_I_CONTROL_PFD_VALUE << 4), MASKBIT(7:4)); // REG_COMBO_PHY1_5B[7:4], port1 i-control PFD overwrite value
            msWriteByteMask(REG_1121B9, BIT1, BIT1); // REG_COMBO_PHY1_5C[9], port1 enable comparator 

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            //msWrite2ByteMask(REG_0017A4, BIT2 |BIT10, BIT2 |BIT10); // REG_PM_BANK_52[2][10], overwrite port2 enable mhl mode, enable mhl mode in sw mode
            msWrite2ByteMask(REG_1122A6, MASKBIT(3:0), MASKBIT(3:0)); // REG_COMBO_PHY2_53[3:0], port2 phase overwrite enable
            msWrite2ByteMask(REG_1122B2, BIT8, BIT8); // REG_COMBO_PHY2_59[8], port2 rterm control overwrite

#if(MHL_RCONTROL_OVERWRITE_ENABLE2)
            if((msReadByte(112200) & BIT5) == BIT5) // REG_COMBO_PHY2_00[5]: RB swap
            {
                msWrite2ByteMask(REG_1122B6, MHL_RTERM_CONTROL_VALUE2 |MASKBIT(2:1), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY2_5B[15:12][3], port2 power down lane, rterm control
            }
            else
            {
                msWrite2ByteMask(REG_1122B6, MHL_RTERM_CONTROL_VALUE2 |MASKBIT(3:2), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY2_5B[15:12][1], port2 power down lane, rterm control
            }
            
#else
            if((msReadByte(112200) & BIT5) == BIT5) // REG_COMBO_PHY2_00[5]: RB swap
            {
                msWrite2ByteMask(REG_1122B6, MHL_RTERM_CONTROL_VALUE |MASKBIT(2:1), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY2_5B[15:12][3], port2 power down lane, rterm control
            }
            else
            {
                msWrite2ByteMask(REG_1122B6, MHL_RTERM_CONTROL_VALUE |MASKBIT(3:2), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY2_5B[15:12][1], port2 power down lane, rterm control
            }
            
#endif

            msWrite2ByteMask(REG_1122DC, MASKBIT(9:6), MASKBIT(9:6)); // REG_COMBO_PHY2_6E[9:6], port2 EQ overwrite
            msWrite2ByteMask(REG_1122E6, MHL_B_CHANNEL_EQ_VALUE, MASKBIT(4:0)); // REG_COMBO_PHY2_73[4:0], port2 EQ overwrite value
            msWrite2ByteMask(REG_161304, 0, BIT1); // REG_COMBO_DECODER2_02[1]: port2 packed-pixel mode enable
            msWrite2ByteMask(REG_16123C, 0, BIT2); // REG_COMBO_HDCP2_1E[2]: port2 packed-pixel mode + HDCP enable
            //msWriteByteMask(REG_1122DF, BIT7, BIT7); // REG_COMBO_PHY2_6F[15], port2 i-control PFD overwrite
            //msWriteByteMask(REG_1122B6, (MHL_I_CONTROL_PFD_VALUE << 4), MASKBIT(7:4)); // REG_COMBO_PHY2_5B[7:4], port2 i-control PFD overwrite value
            msWriteByteMask(REG_1122B9, BIT1, BIT1); // REG_COMBO_PHY2_5C[9], port2 enable comparator 

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            //msWrite2ByteMask(REG_0017A4, BIT3 |BIT11, BIT3 |BIT11); // REG_PM_BANK_52[3][11], overwrite port3 enable mhl mode, enable mhl mode in sw mode
            msWrite2ByteMask(REG_1123A6, MASKBIT(3:0), MASKBIT(3:0)); // REG_COMBO_PHY3_53[3:0], port3 phase overwrite enable
            msWrite2ByteMask(REG_1123B2, BIT8, BIT8); // REG_COMBO_PHY3_59[8], port3 rterm control overwrite

#if(MHL_RCONTROL_OVERWRITE_ENABLE3)
            if((msReadByte(112300) & BIT5) == BIT5) // REG_COMBO_PHY3_00[5]: RB swap
            {
                msWrite2ByteMask(REG_1123B6, MHL_RTERM_CONTROL_VALUE3 |MASKBIT(2:1), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY3_5B[15:12][3], port3 power down lane, rterm control
            }
            else
            {
                msWrite2ByteMask(REG_1123B6, MHL_RTERM_CONTROL_VALUE3 |MASKBIT(3:2), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY3_5B[15:12][1], port3 power down lane, rterm control
            }
            
#else
            if((msReadByte(112300) & BIT5) == BIT5) // REG_COMBO_PHY3_00[5]: RB swap
            {
                msWrite2ByteMask(REG_1123B6, MHL_RTERM_CONTROL_VALUE |MASKBIT(2:1), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY3_5B[15:12][3], port3 power down lane, rterm control
            }
            else
            {
                msWrite2ByteMask(REG_1123B6, MHL_RTERM_CONTROL_VALUE |MASKBIT(3:2), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY3_5B[15:12][1], port3 power down lane, rterm control
            }
            
#endif

            msWrite2ByteMask(REG_1123DC, MASKBIT(9:6), MASKBIT(9:6)); // REG_COMBO_PHY3_6E[9:6], port3 EQ overwrite
            msWrite2ByteMask(REG_1123E6, MHL_B_CHANNEL_EQ_VALUE, MASKBIT(4:0)); // REG_COMBO_PHY3_73[4:0], port3 EQ overwrite value
            msWrite2ByteMask(REG_161B04, 0, BIT1); // REG_COMBO_DECODER3_02[1]: port3 packed-pixel mode enable
            msWrite2ByteMask(REG_161A3C, 0, BIT2); // REG_COMBO_HDCP3_1E[2]: port3 packed-pixel mode + HDCP enable
            //msWriteByteMask(REG_1123DF, BIT7, BIT7); // REG_COMBO_PHY3_6F[15], port3 i-control PFD overwrite
            //msWriteByteMask(REG_1123B6, (MHL_I_CONTROL_PFD_VALUE << 4), MASKBIT(7:4)); // REG_COMBO_PHY3_5B[7:4], port3 i-control PFD overwrite value
            msWriteByteMask(REG_1123B9, BIT1, BIT1); // REG_COMBO_PHY3_5C[9], port3 enable comparator 

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            //msWrite2ByteMask(REG_0017A4, BIT4 |BIT12, BIT4 |BIT12); // REG_PM_BANK_52[4][12], overwrite port4 enable mhl mode, enable mhl mode in sw mode
            msWrite2ByteMask(REG_1124A6, MASKBIT(3:0), MASKBIT(3:0)); // REG_COMBO_PHY4_53[3:0], port4 phase overwrite enable
            msWrite2ByteMask(REG_1124B2, BIT8, BIT8); // REG_COMBO_PHY4_59[8], port4 rterm control overwrite

#if(MHL_RCONTROL_OVERWRITE_ENABLE4)
            if((msReadByte(112400) & BIT5) == BIT5) // REG_COMBO_PHY4_00[5]: RB swap
            {
                msWrite2ByteMask(REG_1124B6, MHL_RTERM_CONTROL_VALUE4 |MASKBIT(2:1), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY4_5B[15:12][3], port4 power down lane, rterm control
            }
            else
            {
                msWrite2ByteMask(REG_1124B6, MHL_RTERM_CONTROL_VALUE4 |MASKBIT(3:2), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY4_5B[15:12][1], port4 power down lane, rterm control
            }
            
#else
            if((msReadByte(112400) & BIT5) == BIT5) // REG_COMBO_PHY4_00[5]: RB swap
            {
                msWrite2ByteMask(REG_1124B6, MHL_RTERM_CONTROL_VALUE |MASKBIT(2:1), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY4_5B[15:12][3], port4 power down lane, rterm control
            }
            else
            {
                msWrite2ByteMask(REG_1124B6, MHL_RTERM_CONTROL_VALUE |MASKBIT(3:2), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY4_5B[15:12][1], port4 power down lane, rterm control
            }
            
#endif

            msWrite2ByteMask(REG_1124DC, MASKBIT(9:6), MASKBIT(9:6)); // REG_COMBO_PHY4_6E[9:6], port4 EQ overwrite
            msWrite2ByteMask(REG_1124E6, MHL_B_CHANNEL_EQ_VALUE, MASKBIT(4:0)); // REG_COMBO_PHY4_73[4:0], port4 EQ overwrite value
            msWrite2ByteMask(REG_162304, 0, BIT1); // REG_COMBO_DECODER4_02[1]: port4 packed-pixel mode enable
            msWrite2ByteMask(REG_16223C, 0, BIT2); // REG_COMBO_HDCP4_1E[2]: port4 packed-pixel mode + HDCP enable
            //msWriteByteMask(REG_1124DF, BIT7, BIT7); // REG_COMBO_PHY4_6F[15], port4 i-control PFD overwrite
            //msWriteByteMask(REG_1124B6, (MHL_I_CONTROL_PFD_VALUE << 4), MASKBIT(7:4)); // REG_COMBO_PHY4_5B[7:4], port4 i-control PFD overwrite value
            msWriteByteMask(REG_1124B9, BIT1, BIT1); // REG_COMBO_PHY4_5C[9], port4 enable comparator 

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            //msWrite2ByteMask(REG_0017A4, BIT5 |BIT13, BIT5 |BIT13); // REG_PM_BANK_52[5][13], overwrite port5 enable mhl mode, enable mhl mode in sw mode
            msWrite2ByteMask(REG_1125A6, MASKBIT(3:0), MASKBIT(3:0)); // REG_COMBO_PHY5_53[3:0], port5 phase overwrite enable
            msWrite2ByteMask(REG_1125B2, BIT8, BIT8); // REG_COMBO_PHY5_59[8], port5 rterm control overwrite

#if(MHL_RCONTROL_OVERWRITE_ENABLE5)
            if((msReadByte(112500) & BIT5) == BIT5) // REG_COMBO_PHY5_00[5]: RB swap
            {
                msWrite2ByteMask(REG_1125B6, MHL_RTERM_CONTROL_VALUE5 |MASKBIT(2:1), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY5_5B[15:12][3], port5 power down lane, rterm control
            }
            else
            {
                msWrite2ByteMask(REG_1125B6, MHL_RTERM_CONTROL_VALUE5 |MASKBIT(3:2), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY5_5B[15:12][1], port5 power down lane, rterm control
            }
            
#else
            if((msReadByte(112500) & BIT5) == BIT5) // REG_COMBO_PHY5_00[5]: RB swap
            {
                msWrite2ByteMask(REG_1125B6, MHL_RTERM_CONTROL_VALUE |MASKBIT(2:1), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY5_5B[15:12][3], port5 power down lane, rterm control
            }
            else
            {
                msWrite2ByteMask(REG_1125B6, MHL_RTERM_CONTROL_VALUE |MASKBIT(3:2), MASKBIT(15:12) |MASKBIT(3:1)); // REG_COMBO_PHY5_5B[15:12][1], port5 power down lane, rterm control
            }
            
#endif

            msWrite2ByteMask(REG_1125DC, MASKBIT(9:6), MASKBIT(9:6)); // REG_COMBO_PHY5_6E[9:6], port5 EQ overwrite
            msWrite2ByteMask(REG_1125E6, MHL_B_CHANNEL_EQ_VALUE, MASKBIT(4:0)); // REG_COMBO_PHY5_73[4:0], port5 EQ overwrite value
            msWrite2ByteMask(REG_162B04, 0, BIT1); // REG_COMBO_DECODER5_02[1]: port5 packed-pixel mode enable
            msWrite2ByteMask(REG_162A3C, 0, BIT2); // REG_COMBO_HDCP5_1E[2]: port5 packed-pixel mode + HDCP enable
            //msWriteByteMask(REG_1125DF, BIT7, BIT7); // REG_COMBO_PHY5_6F[15], port5 i-control PFD overwrite
            //msWriteByteMask(REG_1125B6, (MHL_I_CONTROL_PFD_VALUE << 4), MASKBIT(7:4)); // REG_COMBO_PHY5_5B[7:4], port5 i-control PFD overwrite value
            msWriteByteMask(REG_1125B9, BIT1, BIT1); // REG_COMBO_PHY5_5C[9], port5 enable comparator 

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_PacketPixelModeSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_PacketPixelModeSetting(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWrite2ByteMask(REG_160504, BIT1, BIT1); // REG_COMBO_DECODER0_02[1]: port0 packed-pixel mode enable
            msWrite2ByteMask(REG_16043C, BIT2, BIT2); // REG_COMBO_HDCP0_1E[2]: port0 packed-pixel mode + HDCP enable
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWrite2ByteMask(REG_160B04, BIT1, BIT1); // REG_COMBO_DECODER1_02[1]: port1 packed-pixel mode enable
            msWrite2ByteMask(REG_160A3C, BIT2, BIT2); // REG_COMBO_HDCP1_1E[2]: port1 packed-pixel mode + HDCP enable
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            msWrite2ByteMask(REG_161304, BIT1, BIT1); // REG_COMBO_DECODER2_02[1]: port2 packed-pixel mode enable
            msWrite2ByteMask(REG_16123C, BIT2, BIT2); // REG_COMBO_HDCP2_1E[2]: port2 packed-pixel mode + HDCP enable
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            msWrite2ByteMask(REG_161B04, BIT1, BIT1); // REG_COMBO_DECODER3_02[1]: port3 packed-pixel mode enable
            msWrite2ByteMask(REG_161A3C, BIT2, BIT2); // REG_COMBO_HDCP3_1E[2]: port3 packed-pixel mode + HDCP enable
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            msWrite2ByteMask(REG_162304, BIT1, BIT1); // REG_COMBO_DECODER4_02[1]: port4 packed-pixel mode enable
            msWrite2ByteMask(REG_16223C, BIT2, BIT2); // REG_COMBO_HDCP4_1E[2]: port4 packed-pixel mode + HDCP enable
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            msWrite2ByteMask(REG_162B04, BIT1, BIT1); // REG_COMBO_DECODER5_02[1]: port5 packed-pixel mode enable
            msWrite2ByteMask(REG_162A3C, BIT2, BIT2); // REG_COMBO_HDCP5_1E[2]: port5 packed-pixel mode + HDCP enable
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetCbusReceiveInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetCbusReceiveInterrupt(BYTE ucPortSelect, Bool bFlag)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_002E74, bFlag? 0: BIT1, BIT1); // REG_MHL_CBUS0_3A[1]: receive packet valid mask
            msWriteByteMask(REG_002E74, BIT0, BIT0); // REG_MHL_CBUS0_3A[0]
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_003074, bFlag? 0: BIT1, BIT1); // REG_MHL_CBUS1_3A[1]: receive packet valid mask
            msWriteByteMask(REG_003074, BIT0, BIT0); // REG_MHL_CBUS1_3A[0]
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_003274, bFlag? 0: BIT1, BIT1); // REG_MHL_CBUS2_3A[1]: receive packet valid mask
            msWriteByteMask(REG_003274, BIT0, BIT0); // REG_MHL_CBUS2_3A[0]
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_003474, bFlag? 0: BIT1, BIT1); // REG_MHL_CBUS3_3A[1]: receive packet valid mask
            msWriteByteMask(REG_003474, BIT0, BIT0); // REG_MHL_CBUS3_3A[0]
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_003674, bFlag? 0: BIT1, BIT1); // REG_MHL_CBUS4_3A[1]: receive packet valid mask
            msWriteByteMask(REG_003674, BIT0, BIT0); // REG_MHL_CBUS4_3A[0]
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            msWriteByteMask(REG_003874, bFlag? 0: BIT1, BIT1); // REG_MHL_CBUS5_3A[1]: receive packet valid mask
            msWriteByteMask(REG_003874, BIT0, BIT0); // REG_MHL_CBUS5_3A[0]
            
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetStucktoLowInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetStucktoLowInterrupt(BYTE ucPortSelect, Bool bFlag)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_002F30, bFlag? 0: BIT1, BIT1); // REG_PM_MHL_CBUS0_18[1]: cbus stuck to low int mask
            msWriteByteMask(REG_002F30, BIT0, BIT0); // REG_PM_MHL_CBUS0_18[0]
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_003130, bFlag? 0: BIT1, BIT1); // REG_PM_MHL_CBUS1_18[1]: cbus stuck to low int mask
            msWriteByteMask(REG_003130, BIT0, BIT0); // REG_PM_MHL_CBUS1_18[0]
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_003330, bFlag? 0: BIT1, BIT1); // REG_PM_MHL_CBUS2_18[1]: cbus stuck to low int mask
            msWriteByteMask(REG_003330, BIT0, BIT0); // REG_PM_MHL_CBUS2_18[0]
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_003530, bFlag? 0: BIT1, BIT1); // REG_PM_MHL_CBUS3_18[1]: cbus stuck to low int mask
            msWriteByteMask(REG_003530, BIT0, BIT0); // REG_PM_MHL_CBUS3_18[0]
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_003730, bFlag? 0: BIT1, BIT1); // REG_PM_MHL_CBUS4_18[1]: cbus stuck to low int mask
            msWriteByteMask(REG_003730, BIT0, BIT0); // REG_PM_MHL_CBUS4_18[0]
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            msWriteByteMask(REG_003930, bFlag? 0: BIT1, BIT1); // REG_PM_MHL_CBUS5_18[1]: cbus stuck to low int mask
            msWriteByteMask(REG_003930, BIT0, BIT0); // REG_PM_MHL_CBUS5_18[0]
            
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ClockBigChangeFlag()
//  [Description]:
//                  
//  [Arguments]:
//                  
//  [Return]:
//                 
//**************************************************************************
Bool _mhal_mhl_ClockBigChangeFlag(BYTE ucPortSelect)
{
    Bool bBigChange = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            bBigChange = ((msReadByte(REG_160656) & BIT4) ? TRUE : FALSE); // REG_COMBO_ATOP0_2B[4]

            if(bBigChange)
            {
                msWriteByteMask(REG_16065A, BIT4, BIT4); // REG_COMBO_ATOP0_2D[4]
                msWriteByteMask(REG_16065A, 0, BIT4); // REG_COMBO_ATOP0_2D[4]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            bBigChange = ((msReadByte(REG_160C56) & BIT4) ? TRUE : FALSE); // REG_COMBO_ATOP1_2B[4]

            if(bBigChange)
            {
                msWriteByteMask(REG_160C5A, BIT4, BIT4); // REG_COMBO_ATOP1_2D[4]
                msWriteByteMask(REG_160C5A, 0, BIT4); // REG_COMBO_ATOP1_2D[4]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            bBigChange = ((msReadByte(REG_161456) & BIT4) ? TRUE : FALSE); // REG_COMBO_ATOP2_2B[4]

            if(bBigChange)
            {
                msWriteByteMask(REG_16145A, BIT4, BIT4); // REG_COMBO_ATOP2_2D[4]
                msWriteByteMask(REG_16145A, 0, BIT4); // REG_COMBO_ATOP2_2D[4]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            bBigChange = ((msReadByte(REG_161C56) & BIT4) ? TRUE : FALSE); // REG_COMBO_ATOP3_2B[4]

            if(bBigChange)
            {
                msWriteByteMask(REG_161C5A, BIT4, BIT4); // REG_COMBO_ATOP3_2D[4]
                msWriteByteMask(REG_161C5A, 0, BIT4); // REG_COMBO_ATOP3_2D[4]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            bBigChange = ((msReadByte(REG_162456) & BIT4) ? TRUE : FALSE); // REG_COMBO_ATOP4_2B[4]

            if(bBigChange)
            {
                msWriteByteMask(REG_16245A, BIT4, BIT4); // REG_COMBO_ATOP4_2D[4]
                msWriteByteMask(REG_16245A, 0, BIT4); // REG_COMBO_ATOP4_2D[4]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            bBigChange = ((msReadByte(REG_162C56) & BIT4) ? TRUE : FALSE); // REG_COMBO_ATOP5_2B[4]

            if(bBigChange)
            {
                msWriteByteMask(REG_162C5A, BIT4, BIT4); // REG_COMBO_ATOP5_2D[4]
                msWriteByteMask(REG_162C5A, 0, BIT4); // REG_COMBO_ATOP5_2D[4]
            }
            
            break;
#endif

        default:

            break;
    };

    return bBigChange;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CheckClockStatus()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mhal_mhl_CheckClockStatus(BYTE ucPortSelect)
{
    Bool bStable = FALSE;
    Bool bClockChange = _mhal_mhl_ClockBigChangeFlag(ucPortSelect);
    WORD usClkCount = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(msReadByte(REG_112083) & BIT1) // REG_COMBO_PHY0_41[9], Clock stable flag.
            {
                usClkCount = msRead2Byte(REG_112078) &MASKBIT(11:0); // REG_COMBO_PHY0_3C[11:0], Clock counter.

                bStable = TRUE;
                
                if(bClockChange && (usClkCount < CBUS_CLOCK_DETECT_LEVEL))
                {
                    bStable = FALSE;
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(msReadByte(REG_112183) & BIT1) // REG_COMBO_PHY1_41[9], Clock stable flag.
            {
                usClkCount = msRead2Byte(REG_112178) &MASKBIT(11:0); // REG_COMBO_PHY1_3C[11:0], Clock counter.

                bStable = TRUE;
                
                if(bClockChange && (usClkCount < CBUS_CLOCK_DETECT_LEVEL))
                {
                    bStable = FALSE;
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(msReadByte(REG_112283) & BIT1) // REG_COMBO_PHY2_41[9], Clock stable flag.
            {
                usClkCount = msRead2Byte(REG_112278) &MASKBIT(11:0); // REG_COMBO_PHY2_3C[11:0], Clock counter.

                bStable = TRUE;
                
                if(bClockChange && (usClkCount < CBUS_CLOCK_DETECT_LEVEL))
                {
                    bStable = FALSE;
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(msReadByte(REG_112383) & BIT1) // REG_COMBO_PHY3_41[9], Clock stable flag.
            {
                usClkCount = msRead2Byte(REG_112378) &MASKBIT(11:0); // REG_COMBO_PHY3_3C[11:0], Clock counter.

                bStable = TRUE;
                
                if(bClockChange && (usClkCount < CBUS_CLOCK_DETECT_LEVEL))
                {
                    bStable = FALSE;
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(msReadByte(REG_112483) & BIT1) // REG_COMBO_PHY4_41[9], Clock stable flag.
            {
                usClkCount = msRead2Byte(REG_112478) &MASKBIT(11:0); // REG_COMBO_PHY4_3C[11:0], Clock counter.

                bStable = TRUE;
                
                if(bClockChange && (usClkCount < CBUS_CLOCK_DETECT_LEVEL))
                {
                    bStable = FALSE;
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(msReadByte(REG_112583) & BIT1) // REG_COMBO_PHY5_41[9], Clock stable flag.
            {
                usClkCount = msRead2Byte(REG_112578) &MASKBIT(11:0); // REG_COMBO_PHY5_3C[11:0], Clock counter.

                bStable = TRUE;
                
                if(bClockChange && (usClkCount < CBUS_CLOCK_DETECT_LEVEL))
                {
                    bStable = FALSE;
                }
            }
            
            break;
#endif

        default:

            break;
    };

    return bStable;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CheckDataEnableStatus()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool _mhal_mhl_CheckDataEnableStatus(BYTE ucPortSelect)
{
    Bool bStable = FALSE;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(msReadByte(REG_16045D) & BIT6) // REG_COMBO_HDCP0_2E[14]: DE stable
            {
                bStable = TRUE;
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(msReadByte(REG_160A5D) & BIT6) // REG_COMBO_HDCP1_2E[14]: DE stable
            {
                bStable = TRUE;
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(msReadByte(REG_16125D) & BIT6) // REG_COMBO_HDCP2_2E[14]: DE stable
            {
                bStable = TRUE;
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(msReadByte(REG_161A5D) & BIT6) // REG_COMBO_HDCP3_2E[14]: DE stable
            {
                bStable = TRUE;
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(msReadByte(REG_16225D) & BIT6) // REG_COMBO_HDCP4_2E[14]: DE stable
            {
                bStable = TRUE;
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(msReadByte(REG_162A5D) & BIT6) // REG_COMBO_HDCP5_2E[14]: DE stable
            {
                bStable = TRUE;
            }
            
            break;
#endif

        default:

            break;
    };

    return bStable;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_TrimPullDown1K()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_TrimPullDown1K(BYTE ucPD1KIndex)
{
    msWriteByteMask(REG_0017B4, ucPD1KIndex, MASKBIT(1:0)); // REG_PM_BANK_5A[1:0], Cbus pull down 1K
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_TrimPullDown100K()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_TrimPullDown100K(BYTE ucPD100KIndex)
{
#if(MHL_FUNCTION_SUPPORT_PORT0)
    msWriteByteMask(REG_1120F8, ucPD100KIndex, MASKBIT(2:1)); // REG_COMBO_PHY0_7C[2:1]: Cbus pull down 100K port0
#endif
#if(MHL_FUNCTION_SUPPORT_PORT1)
    msWriteByteMask(REG_1121F8, ucPD100KIndex, MASKBIT(2:1)); // REG_COMBO_PHY1_7C[2:1]: Cbus pull down 100K port1
#endif
#if(MHL_FUNCTION_SUPPORT_PORT2)
    msWriteByteMask(REG_1122F8, ucPD100KIndex, MASKBIT(2:1)); // REG_COMBO_PHY2_7C[2:1]: Cbus pull down 100K port2
#endif
#if(MHL_FUNCTION_SUPPORT_PORT3)
    msWriteByteMask(REG_1123F8, ucPD100KIndex, MASKBIT(2:1)); // REG_COMBO_PHY3_7C[2:1]: Cbus pull down 100K port3
#endif
#if(MHL_FUNCTION_SUPPORT_PORT4)
    msWriteByteMask(REG_1124F8, ucPD100KIndex, MASKBIT(2:1)); // REG_COMBO_PHY4_7C[2:1]: Cbus pull down 100K port4
#endif
#if(MHL_FUNCTION_SUPPORT_PORT5)
    msWriteByteMask(REG_1125F8, ucPD100KIndex, MASKBIT(2:1)); // REG_COMBO_PHY5_7C[2:1]: Cbus pull down 100K port5
#endif
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetPadPullUp1K()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetPadPullUp1K(BYTE ucPortSelect, Bool bFlag)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            //msWriteByteMask(REG_000468, bFlag? BIT0: 0, BIT0); // REG_PAD_TOP_34[0], Enable GPIOX02 pull-up 1k
            break;

        case MHL_INPUT_SELECT_PORT1:
            //msWriteByteMask(REG_000468, bFlag? BIT1: 0, BIT1); // REG_PAD_TOP_34[1], Enable GPIOX12 pull-up 1k
            break;

        case MHL_INPUT_SELECT_PORT2:
            //msWriteByteMask(REG_000468, bFlag? BIT2: 0, BIT2); // REG_PAD_TOP_34[2], Enable GPIOX22 pull-up 1k
            break;

        case MHL_INPUT_SELECT_PORT3:
            //msWriteByteMask(REG_000468, bFlag? BIT3: 0, BIT3); // REG_PAD_TOP_34[3], Enable GPIOX32 pull-up 1k
            break;

        case MHL_INPUT_SELECT_PORT4:
            //msWriteByteMask(REG_000468, bFlag? BIT4: 0, BIT4); // REG_PAD_TOP_34[4], Enable GPIOX42 pull-up 1k
            break;

        case MHL_INPUT_SELECT_PORT5:
            //msWriteByteMask(REG_000468, bFlag? BIT5: 0, BIT5); // REG_PAD_TOP_34[5], Enable GPIOX52 pull-up 1k
            break;
            
        default:
            bFlag = 0;
            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetCbusPullDown100K()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetCbusPullDown100K(BYTE ucPortSelect, Bool bSetFlag)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_002F40, bSetFlag? BIT7: 0, BIT7); // REG_PM_MHL_CBUS0_20[7], Cbus pull down 100K port0
            break;

        case MHL_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_003140, bSetFlag? BIT7: 0, BIT7); // REG_PM_MHL_CBUS1_20[7], Cbus pull down 100K port1
            break;

        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_003340, bSetFlag? BIT7: 0, BIT7); // REG_PM_MHL_CBUS2_20[7], Cbus pull down 100K port2
            break;

        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_003540, bSetFlag? BIT7: 0, BIT7); // REG_PM_MHL_CBUS3_20[7], Cbus pull down 100K port3
            break;

        case MHL_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_003740, bSetFlag? BIT7: 0, BIT7); // REG_PM_MHL_CBUS4_20[7], Cbus pull down 100K port3
            break;

        case MHL_INPUT_SELECT_PORT5:
            msWriteByteMask(REG_003940, bSetFlag? BIT7: 0, BIT7); // REG_PM_MHL_CBUS5_20[7], Cbus pull down 100K port3
            break;
            
        default:

            break;
    };
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_EnableCbusInterrupt()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//
//**************************************************************************
void mhal_mhl_EnableCbusInterrupt(Bool bEnableInterrupt)
{
    msWriteByteMask(REG_002B29, bEnableInterrupt? 0: BIT4, BIT4); // REG_IRQ_BANK_14[12]
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetCbusInterruptFlag()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//
//**************************************************************************
Bool mhal_mhl_GetCbusInterruptFlag(void)
{
    return ((msReadByte(REG_002B39) & BIT4) ? TRUE : FALSE); // REG_IRQ_BANK_19[12]
}
//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_LoadEDID()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_LoadEDID(BYTE ucPortSelect, BYTE *pComboEDID)
{
    WORD uctemp = 0; 

    if(pComboEDID != NULL)
    {
        MHL_HAL_DPRINTF("** MHL load EDID... port %d", ucPortSelect);
        
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                msWriteByteMask(REG_002EA4, BIT1, BIT1); // REG_MHL_CBUS0_52[1], CPU write enable
                
                for(uctemp = 0; uctemp <256; uctemp++)
                {        
                    msWriteByte(REG_002EA5, uctemp); // REG_MHL_CBUS0_52[15:8], address        
                    msWriteByte(REG_002EA6, pComboEDID[uctemp]); // REG_MHL_CBUS0_53[7:0], data
                    msWriteByteMask(REG_002EA4, BIT0, BIT0); // REG_MHL_CBUS0_52[0], Cpu write EDID SRAM triger.   
                    msWriteByteMask(REG_002EA4, 0, BIT0);
                    while(msReadByte(REG_002EA4) & BIT5); // REG_MHL_CBUS0_52[5], is write data busy       
                }    

                msWriteByteMask(REG_002EA4, 0, BIT1); // REG_MHL_CBUS0_52[1], CPU write disable
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                msWriteByteMask(REG_0030A4, BIT1, BIT1); // REG_MHL_CBUS1_52[1], CPU write enable
                
                for(uctemp = 0; uctemp <256; uctemp++)
                {        
                    msWriteByte(REG_0030A5, uctemp); // REG_MHL_CBUS1_52[15:8], address        
                    msWriteByte(REG_0030A6, pComboEDID[uctemp]); // REG_MHL_CBUS1_53[7:0], data
                    msWriteByteMask(REG_0030A4, BIT0, BIT0); // REG_MHL_CBUS1_52[0], Cpu write EDID SRAM triger.   
                    msWriteByteMask(REG_0030A4, 0, BIT0);
                    while(msReadByte(REG_0030A4) & BIT5); // REG_MHL_CBUS1_52[5], is write data busy       
                }    

                msWriteByteMask(REG_0030A4, 0, BIT1); // REG_MHL_CBUS1_52[1], CPU write disable
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
            case MHL_INPUT_SELECT_PORT2:
                msWriteByteMask(REG_0032A4, BIT1, BIT1); // REG_MHL_CBUS2_52[1], CPU write enable
                
                for(uctemp = 0; uctemp <256; uctemp++)
                {        
                    msWriteByte(REG_0032A5, uctemp); // REG_MHL_CBUS2_52[15:8], address        
                    msWriteByte(REG_0032A6, pComboEDID[uctemp]); // REG_MHL_CBUS2_53[7:0], data
                    msWriteByteMask(REG_0032A4, BIT0, BIT0); // REG_MHL_CBUS2_52[0], Cpu write EDID SRAM triger.   
                    msWriteByteMask(REG_0032A4, 0, BIT0);
                    while(msReadByte(REG_0032A4) & BIT5); // REG_MHL_CBUS2_52[5], is write data busy       
                }    

                msWriteByteMask(REG_0032A4, 0, BIT1); // REG_MHL_CBUS2_52[1], CPU write disable
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
            case MHL_INPUT_SELECT_PORT3:
                msWriteByteMask(REG_0034A4, BIT1, BIT1); // REG_MHL_CBUS3_52[1], CPU write enable
                
                for(uctemp = 0; uctemp <256; uctemp++)
                {        
                    msWriteByte(REG_0034A5, uctemp); // REG_MHL_CBUS3_52[15:8], address        
                    msWriteByte(REG_0034A6, pComboEDID[uctemp]); // REG_MHL_CBUS3_53[7:0], data
                    msWriteByteMask(REG_0034A4, BIT0, BIT0); // REG_MHL_CBUS3_52[0], Cpu write EDID SRAM triger.   
                    msWriteByteMask(REG_0034A4, 0, BIT0);
                    while(msReadByte(REG_0034A4) & BIT5); // REG_MHL_CBUS3_52[5], is write data busy       
                }    

                msWriteByteMask(REG_0034A4, 0, BIT1); // REG_MHL_CBUS3_52[1], CPU write disable
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
            case MHL_INPUT_SELECT_PORT4:
                msWriteByteMask(REG_0036A4, BIT1, BIT1); // REG_MHL_CBUS4_52[1], CPU write enable
                
                for(uctemp = 0; uctemp <256; uctemp++)
                {        
                    msWriteByte(REG_0036A5, uctemp); // REG_MHL_CBUS4_52[15:8], address        
                    msWriteByte(REG_0036A6, pComboEDID[uctemp]); // REG_MHL_CBUS4_53[7:0], data
                    msWriteByteMask(REG_0036A4, BIT0, BIT0); // REG_MHL_CBUS4_52[0], Cpu write EDID SRAM triger.   
                    msWriteByteMask(REG_0036A4, 0, BIT0);
                    while(msReadByte(REG_0036A4) & BIT5); // REG_MHL_CBUS4_52[5], is write data busy       
                }    

                msWriteByteMask(REG_0036A4, 0, BIT1); // REG_MHL_CBUS4_52[1], CPU write disable
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
            case MHL_INPUT_SELECT_PORT5:
                msWriteByteMask(REG_0038A4, BIT1, BIT1); // REG_MHL_CBUS5_52[1], CPU write enable
                
                for(uctemp = 0; uctemp <256; uctemp++)
                {        
                    msWriteByte(REG_0038A5, uctemp); // REG_MHL_CBUS5_52[15:8], address        
                    msWriteByte(REG_0038A6, pComboEDID[uctemp]); // REG_MHL_CBUS5_53[7:0], data
                    msWriteByteMask(REG_0038A4, BIT0, BIT0); // REG_MHL_CBUS5_52[0], Cpu write EDID SRAM triger.   
                    msWriteByteMask(REG_0038A4, 0, BIT0);
                    while(msReadByte(REG_0038A4) & BIT5); // REG_MHL_CBUS5_52[5], is write data busy       
                }    

                msWriteByteMask(REG_0038A4, 0, BIT1); // REG_MHL_CBUS5_52[1], CPU write disable
                
                break;
#endif

            default:

                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_Initial()
//  [Description]
//                  MHL init
//  [Arguments]:
//                  *edid: MHL EDID data
//                  *devcap: MHL device capability
//  [Return]:
//
//**************************************************************************
void mhal_mhl_Initial(BYTE ucPortSelect, Bool bPowerIgnore, BYTE *pComboEDID, BYTE *pDevcap, BYTE ucVenderID)
{
    if(!bPowerIgnore)
    {
        // Cbus pad config
        _mhal_mhl_CbusPadSetting(ucPortSelect);
        // Load EDID
        mhal_mhl_LoadEDID(ucPortSelect, pComboEDID);
        // Load device capability and Vendor ID 
        _mhal_mhl_LoadDevcapVenderID(ucPortSelect, pDevcap, ucVenderID);
        // Enable receive MSC interrupt
        _mhal_mhl_SetCbusReceiveInterrupt(ucPortSelect, TRUE);
    }
    else
    {
        // Set pad PU1K
        _mhal_mhl_SetPadPullUp1K(ucPortSelect, TRUE);
    }

    // Set Cbus pull down 100K
    _mhal_mhl_SetCbusPullDown100K(ucPortSelect, TRUE);
    // Disable stuck to low interrupt
    _mhal_mhl_SetStucktoLowInterrupt(ucPortSelect, FALSE);
    // Phy setting
    _mhal_mhl_ComboPhySetting(ucPortSelect);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SetPullDownResistor()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_SetPullDownResistor(BYTE ucEfuseIndex)
{
    BYTE ucPD1KIndex = 0;
    BYTE ucPD100KIndex = 0;
    
    if((ucEfuseIndex & MHL_CBUS_EFUSE_TRIMMING) == MHL_CBUS_EFUSE_TRIMMING)
    {
        if((ucEfuseIndex & MHL_CBUS_EFUSE_PD_1K_HIGH) == MHL_CBUS_EFUSE_PD_1K_HIGH)
        {
            ucPD1KIndex |= MHL_CBUS_PD_1K_HIGH_FLAG;
        }

        if((ucEfuseIndex & MHL_CBUS_EFUSE_PD_1K_LOW) == MHL_CBUS_EFUSE_PD_1K_LOW)
        {
            ucPD1KIndex |= MHL_CBUS_PD_1K_LOW_FLAG;
        }

        _mhal_mhl_TrimPullDown1K(ucPD1KIndex);

        if((ucEfuseIndex & MHL_CBUS_EFUSE_PD_100K_HIGH) == MHL_CBUS_EFUSE_PD_100K_HIGH)
        {
            ucPD100KIndex |= MHL_CBUS_PD_100K_HIGH_FLAG;
        }

        if((ucEfuseIndex & MHL_CBUS_EFUSE_PD_100K_LOW) == MHL_CBUS_EFUSE_PD_100K_LOW)
        {
            ucPD100KIndex |= MHL_CBUS_PD_100K_LOW_FLAG;
        }

        _mhal_mhl_TrimPullDown100K(ucPD100KIndex);
    }
}
//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SAR_Detect()
//  [Description]
//                  MHL SAR detection
//  [Arguments]:
//                  
//  [Return]:
//                  TRUE: MHL SAR plugged
//                  FALSE: MHL SAR unplugged
//**************************************************************************
Bool mhal_mhl_SAR_Detect(BYTE ucPortSelect)
{
    Bool bCableDetect = FALSE;

    // Use SAR to check MHL cable attach
    switch(ucPortSelect)
    {
    #if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if (MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MAX_TARGET)>=MHL_CABLE_SAR_DETECT_PORT0 && MHL_CABLE_SAR_DETECT_PORT0 >= MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MIN_TARGET))
                bCableDetect = TRUE;            
            break;
    #endif

    #if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if (MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MAX_TARGET)>=MHL_CABLE_SAR_DETECT_PORT1 && MHL_CABLE_SAR_DETECT_PORT1 >= MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MIN_TARGET))
                bCableDetect = TRUE;            
            break;
    #endif

    #if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if (MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MAX_TARGET)>=MHL_CABLE_SAR_DETECT_PORT2 && MHL_CABLE_SAR_DETECT_PORT2 >= MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MIN_TARGET))
                bCableDetect = TRUE;
            break;
    #endif

    #if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if (MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MAX_TARGET)>=MHL_CABLE_SAR_DETECT_PORT3 && MHL_CABLE_SAR_DETECT_PORT3 >= MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MIN_TARGET))
                bCableDetect = TRUE;
            break;
    #endif

    #if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if (MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MAX_TARGET)>=MHL_CABLE_SAR_DETECT_PORT4 && MHL_CABLE_SAR_DETECT_PORT4 >= MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MIN_TARGET))
                bCableDetect = TRUE;
            break;
    #endif

    #if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if (MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MAX_TARGET)>=MHL_CABLE_SAR_DETECT_PORT5 && MHL_CABLE_SAR_DETECT_PORT5 >= MHL_CABLE_DETECT_LEVEL(MHL_SAR_VALTAGE_MIN_TARGET))
                bCableDetect = TRUE;
            break;
    #endif

        default:
            break;
    };

    return bCableDetect;
}
//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CableDetect()
//  [Description]
//                  MHL cable detection
//  [Arguments]:
//                  
//  [Return]:
//                  TRUE: MHL cable plugged
//                  FALSE: MHL cable unplugged
//**************************************************************************
Bool mhal_mhl_CableDetect(BYTE ucPortSelect)
{
    Bool bCableDetect = FALSE;

    // Use SAR to check MHL cable attach
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(GET_MHL_CABLE_DETECT_ENABLE0() && mhal_mhl_SAR_Detect(ucPortSelect))
            {
                bCableDetect = TRUE;
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(GET_MHL_CABLE_DETECT_ENABLE1() && mhal_mhl_SAR_Detect(ucPortSelect))
            {
                bCableDetect = TRUE;
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(GET_MHL_CABLE_DETECT_ENABLE2() && mhal_mhl_SAR_Detect(ucPortSelect))
            {
                bCableDetect = TRUE;
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(GET_MHL_CABLE_DETECT_ENABLE3() && mhal_mhl_SAR_Detect(ucPortSelect))
            {
                bCableDetect = TRUE;
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(GET_MHL_CABLE_DETECT_ENABLE4() && mhal_mhl_SAR_Detect(ucPortSelect))
            {
                bCableDetect = TRUE;
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(GET_MHL_CABLE_DETECT_ENABLE5() && mhal_mhl_SAR_Detect(ucPortSelect))
            {
                bCableDetect = TRUE;
            }
            
            break;
#endif

        default:

            break;
    };

    return bCableDetect;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusConnectStatus()
//  [Description]:
//                  MHL Cbus status
//  [Arguments]:
//
//  [Return]:
//                  Cbus status value      
//**************************************************************************
Bool mhal_mhl_CbusConnectStatus(BYTE ucPortSelect)
{
    Bool bCbusConnect = FALSE;
    BYTE ucCbusStatus = 0;

    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            ucCbusStatus = msReadByte(REG_002F2E); // REG_PM_MHL_CBUS0_17[1:0], link layer status
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            ucCbusStatus = msReadByte(REG_00312E); // REG_PM_MHL_CBUS1_17[1:0], link layer status
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            ucCbusStatus = msReadByte(REG_00332E); // REG_PM_MHL_CBUS2_17[1:0], link layer status
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            ucCbusStatus = msReadByte(REG_00352E); // REG_PM_MHL_CBUS3_17[1:0], link layer status
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            ucCbusStatus = msReadByte(REG_00372E); // REG_PM_MHL_CBUS4_17[1:0], link layer status
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            ucCbusStatus = msReadByte(REG_00392E); // REG_PM_MHL_CBUS5_17[1:0], link layer status
            break;
#endif

        default:

            break;
    };

    if(ucCbusStatus == 0x03)
    {
        bCbusConnect = TRUE;
    }

    return bCbusConnect;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetCbusBusyFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_mhl_GetCbusBusyFlag(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            bFlag = ((msReadByte(REG_002EBA) & 0xF0) != 0 ? TRUE : FALSE); // REG_MHL_CBUS0_5D[7:4]
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            bFlag = ((msReadByte(REG_0030BA) & 0xF0) != 0 ? TRUE : FALSE); // REG_MHL_CBUS1_5D[7:4]
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            bFlag = ((msReadByte(REG_0032BA) & 0xF0) != 0 ? TRUE : FALSE); // REG_MHL_CBUS2_5D[7:4]
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            bFlag = ((msReadByte(REG_0034BA) & 0xF0) != 0 ? TRUE : FALSE); // REG_MHL_CBUS3_5D[7:4]
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            bFlag = ((msReadByte(REG_0036BA) & 0xF0) != 0 ? TRUE : FALSE); // REG_MHL_CBUS4_5D[7:4]
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            bFlag = ((msReadByte(REG_0038BA) & 0xF0) != 0 ? TRUE : FALSE); // REG_MHL_CBUS5_5D[7:4]
            break;
#endif

        default:

            break;
    };

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_ComboRtermControl()
//  [Description]
//                  MHL TMDS termination resistor control
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_ComboRtermControl(BYTE ucPortSelect, RXRtermControl_T ucControl) 
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(ucControl == RX_HDMI_RTERM)
            {
                msWriteByteMask(REG_0017C4, 0, MASKBIT(3:0)); // REG_PM_BANK_62[3:0], Power on port0 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                msWriteByteMask(REG_0017C4, 0, MASKBIT(2:1)); // REG_PM_BANK_62[3:0], Power on port0's ch0 rterm.
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                msWriteByteMask(REG_0017C4, MASKBIT(2:1), MASKBIT(2:1)); // REG_PM_BANK_62[3:0], Power down port0 rterm.
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(ucControl == RX_HDMI_RTERM)
            {
                msWriteByteMask(REG_0017C4, 0, MASKBIT(7:4)); // REG_PM_BANK_62[7:4], Power on port1 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                msWriteByteMask(REG_0017C4, 0, MASKBIT(6:5)); // REG_PM_BANK_62[7:4], Power on port1's ch0 rterm.
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                msWriteByteMask(REG_0017C4, MASKBIT(6:5), MASKBIT(6:5)); // REG_PM_BANK_62[7:4], Power down port1 rterm.
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(ucControl == RX_HDMI_RTERM)
            {
                msWriteByteMask(REG_0017C5, 0, MASKBIT(3:0)); // REG_PM_BANK_62[11:8], Power on port2 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                msWriteByteMask(REG_0017C5, 0, MASKBIT(2:1)); // REG_PM_BANK_62[11:8], Power on port2's ch0 rterm.
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                msWriteByteMask(REG_0017C5, MASKBIT(2:1), MASKBIT(2:1)); // REG_PM_BANK_62[11:8], Power down port2 rterm.
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(ucControl == RX_HDMI_RTERM)
            {
                msWriteByteMask(REG_0017A2, 0, MASKBIT(3:0)); // REG_PM_BANK_51[3:0], Power on port3 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                msWriteByteMask(REG_0017A2, 0, MASKBIT(2:1)); // REG_PM_BANK_51[3:0], Power on port3's ch0 rterm.
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                msWriteByteMask(REG_0017A2, MASKBIT(2:1), MASKBIT(2:1)); // REG_PM_BANK_51[3:0], Power down port3 rterm.
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(ucControl == RX_HDMI_RTERM)
            {
                msWriteByteMask(REG_0017A2, 0, MASKBIT(7:4)); // REG_PM_BANK_51[7:4], Power on port4 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                msWriteByteMask(REG_0017A2, 0, MASKBIT(6:5)); // REG_PM_BANK_51[7:4], Power on port4's ch0 rterm.
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                msWriteByteMask(REG_0017A2, MASKBIT(6:5), MASKBIT(6:5)); // REG_PM_BANK_51[7:4], Power down port4 rterm.
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(ucControl == RX_HDMI_RTERM)
            {
                msWriteByteMask(REG_0017A3, 0, MASKBIT(3:0)); // REG_PM_BANK_51[11:8], Power on port5 rterm.
            }
            else if(ucControl == RX_MHL_RTERM)
            {
                msWriteByteMask(REG_0017A3, 0, MASKBIT(2:1)); // REG_PM_BANK_51[11:8], Power on port5's ch0 rterm.
            }
            else if(ucControl == RX_RTERM_OFF)
            {
                msWriteByteMask(REG_0017A3, MASKBIT(2:1), MASKBIT(2:1)); // REG_PM_BANK_51[11:8], Power down port5 rterm.
            }
            
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_RtermControlHWMode()
//  [Description]:
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_RtermControlHWMode(BYTE ucPortSelect, Bool bRtermHW)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_0017C9, bRtermHW? BIT4: 0, BIT4); // REG_PM_BANK_64[12], Cbus control port0 rterm.
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_0017C9, bRtermHW? BIT5: 0, BIT5); // REG_PM_BANK_64[13], Cbus control port1 rterm.
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_0017C9, bRtermHW? BIT6: 0, BIT6); // REG_PM_BANK_64[14], Cbus control port2 rterm.
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_0017A0, bRtermHW? BIT0: 0, BIT0); // REG_PM_BANK_50[0], Cbus control port3 rterm.
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_0017A0, bRtermHW? BIT1: 0, BIT1); // REG_PM_BANK_50[1], Cbus control port4 rterm.
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            msWriteByteMask(REG_0017A0, bRtermHW? BIT2: 0, BIT2); // REG_PM_BANK_50[2], Cbus control port5 rterm.
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CDRModeMonitor()
//  [Description]:
//                  MHL CDR mode monitor
//  [Arguments]:
//                  isCbusConnected: Cbus is at connected state or not
//  [Return]:
//                 
//**************************************************************************
void mhal_mhl_CDRStableMonitor(BYTE ucPortSelect, Bool bDisplayReady)
{
    static BYTE ucStableCount[6] = {0, 0, 0, 0, 0, 0};
    
    if(bDisplayReady && (_mhal_mhl_CheckClockStatus(ucPortSelect)))
    {
        if(ucStableCount[ucPortSelect] < (MHL_CDR_FORCE_THRESHOLD +1))
        {
            ucStableCount[ucPortSelect]++;
        }

        if(_mhal_mhl_CheckDataEnableStatus(ucPortSelect))
        {
            if(!bSignalStable[ucPortSelect])
            {
                bSignalStable[ucPortSelect] = TRUE;
            }
        }
    }
    else if(bSignalStable[ucPortSelect])
    {
        bSignalStable[ucPortSelect] = FALSE;
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusWriteCommand()
//  [Description]
//                  MHL Cbus write trigger
//  [Arguments]:
//                  *pdatabuf: Cbus tx data
//  [Return]:
//
//**************************************************************************
Bool mhal_mhl_CbusWriteCommand(BYTE ucPortSelect, mhalCbusFifo_S *pdatabuf)
{
    Bool bFlag = FALSE;
    BYTE uctemp = 0;

    if(!mhal_mhl_GetCbusBusyFlag(ucPortSelect))
    {
        switch(ucPortSelect)
        {
#if(MHL_FUNCTION_SUPPORT_PORT0)
            case MHL_INPUT_SELECT_PORT0:
                for(uctemp = 0; uctemp <pdatabuf->lens; uctemp++)
                {
                    msWrite2Byte(REG_002E4C +uctemp *2, pdatabuf->databuf[uctemp]); // REG_MHL_CBUS0_26
                }
                
                // clear the unsed parts
                msWrite2Byte(REG_002E4C +((pdatabuf->lens) *2), 0);

                msWriteByteMask(REG_002E4B, BIT4, BIT4); // REG_MHL_CBUS0_25[12], trigger to send
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
            case MHL_INPUT_SELECT_PORT1:
                for(uctemp = 0; uctemp <pdatabuf->lens; uctemp++)
                {
                    msWrite2Byte(REG_00304C +uctemp *2, pdatabuf->databuf[uctemp]); // REG_MHL_CBUS1_26
                }
                
                // clear the unsed parts
                msWrite2Byte(REG_00304C +((pdatabuf->lens) *2), 0);

                msWriteByteMask(REG_00304B, BIT4, BIT4); // REG_MHL_CBUS1_25[12], trigger to send
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
            case MHL_INPUT_SELECT_PORT2:
                for(uctemp = 0; uctemp <pdatabuf->lens; uctemp++)
                {
                    msWrite2Byte(REG_00324C +uctemp *2, pdatabuf->databuf[uctemp]); // REG_MHL_CBUS2_26
                }
                
                // clear the unsed parts
                msWrite2Byte(REG_00324C +((pdatabuf->lens) *2), 0);

                msWriteByteMask(REG_00324B, BIT4, BIT4); // REG_MHL_CBUS2_25[12], trigger to send
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
            case MHL_INPUT_SELECT_PORT3:
                for(uctemp = 0; uctemp <pdatabuf->lens; uctemp++)
                {
                    msWrite2Byte(REG_00344C +uctemp *2, pdatabuf->databuf[uctemp]); // REG_MHL_CBUS3_26
                }
                
                // clear the unsed parts
                msWrite2Byte(REG_00344C +((pdatabuf->lens) *2), 0);

                msWriteByteMask(REG_00344B, BIT4, BIT4); // REG_MHL_CBUS3_25[12], trigger to send
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
            case MHL_INPUT_SELECT_PORT4:
                for(uctemp = 0; uctemp <pdatabuf->lens; uctemp++)
                {
                    msWrite2Byte(REG_00364C +uctemp *2, pdatabuf->databuf[uctemp]); // REG_MHL_CBUS4_26
                }
                
                // clear the unsed parts
                msWrite2Byte(REG_00364C +((pdatabuf->lens) *2), 0);

                msWriteByteMask(REG_00364B, BIT4, BIT4); // REG_MHL_CBUS4_25[12], trigger to send
                
                break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
            case MHL_INPUT_SELECT_PORT5:
                for(uctemp = 0; uctemp <pdatabuf->lens; uctemp++)
                {
                    msWrite2Byte(REG_00384C +uctemp *2, pdatabuf->databuf[uctemp]); // REG_MHL_CBUS5_26
                }
                
                // clear the unsed parts
                msWrite2Byte(REG_00384C +((pdatabuf->lens) *2), 0);

                msWriteByteMask(REG_00384B, BIT4, BIT4); // REG_MHL_CBUS5_25[12], trigger to send
                
                break;
#endif

            default:

                break;
        };

        bFlag = TRUE;
    }

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusSetPathEnable()
//  [Description]:
//                  MHL Cbus set path enable
//  [Arguments]:
//                  TRUE: Enable
//                  FALSE: Disable
//  [Return]:
//   
//**************************************************************************
void mhal_mhl_CbusSetPathEnable(BYTE ucPortSelect, Bool bSetPathEn)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bSetPathEn) // set state to PATH_EN
            {
                msWriteByteMask(REG_002F2F, BIT4, BIT4); // REG_PM_MHL_CBUS0_17[12], Sw set conn_state path.
            }
            else // clear state to not PATH_EN
            {
                msWriteByteMask(REG_002F2F, BIT5, BIT5); // REG_PM_MHL_CBUS0_17[13], Sw clr conn_state path.
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bSetPathEn) // set state to PATH_EN
            {
                msWriteByteMask(REG_00312F, BIT4, BIT4); // REG_PM_MHL_CBUS1_17[12], Sw set conn_state path.
            }
            else // clear state to not PATH_EN
            {
                msWriteByteMask(REG_00312F, BIT5, BIT5); // REG_PM_MHL_CBUS1_17[13], Sw clr conn_state path.
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(bSetPathEn) // set state to PATH_EN
            {
                msWriteByteMask(REG_00332F, BIT4, BIT4); // REG_PM_MHL_CBUS2_17[12], Sw set conn_state path.
            }
            else // clear state to not PATH_EN
            {
                msWriteByteMask(REG_00332F, BIT5, BIT5); // REG_PM_MHL_CBUS2_17[13], Sw clr conn_state path.
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(bSetPathEn) // set state to PATH_EN
            {
                msWriteByteMask(REG_00352F, BIT4, BIT4); // REG_PM_MHL_CBUS3_17[12], Sw set conn_state path.
            }
            else // clear state to not PATH_EN
            {
                msWriteByteMask(REG_00352F, BIT5, BIT5); // REG_PM_MHL_CBUS3_17[13], Sw clr conn_state path.
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(bSetPathEn) // set state to PATH_EN
            {
                msWriteByteMask(REG_00372F, BIT4, BIT4); // REG_PM_MHL_CBUS4_17[12], Sw set conn_state path.
            }
            else // clear state to not PATH_EN
            {
                msWriteByteMask(REG_00372F, BIT5, BIT5); // REG_PM_MHL_CBUS4_17[13], Sw clr conn_state path.
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(bSetPathEn) // set state to PATH_EN
            {
                msWriteByteMask(REG_00392F, BIT4, BIT4); // REG_PM_MHL_CBUS5_17[12], Sw set conn_state path.
            }
            else // clear state to not PATH_EN
            {
                msWriteByteMask(REG_00392F, BIT5, BIT5); // REG_PM_MHL_CBUS5_17[13], Sw clr conn_state path.
            }
            
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SetRxXdataAddress()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_SetRxXdataAddress(BYTE ucPortSelect, Bool bFirstBlock)
{
    WORD usCommandAddress = 0;
        
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bFirstBlock)
            {
                usCommandAddress = MHL_RX_XRAM_ADDR_FIRST_PORT0;
            }
            else
            {
                usCommandAddress = MHL_RX_XRAM_ADDR_SECOND_PORT0;
            }

            msWrite2ByteMask(REG_002ED6, usCommandAddress, MASKBIT(15:0)); // REG_MHL_CBUS0_6B[15:0], Xdata_write_address.
            msWriteByteMask(REG_002ED9, BIT7, BIT7); // REG_MHL_CBUS0_6C[15], reset xdata write address
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bFirstBlock)
            {
                usCommandAddress = MHL_RX_XRAM_ADDR_FIRST_PORT1;
            }
            else
            {
                usCommandAddress = MHL_RX_XRAM_ADDR_SECOND_PORT1;
            }

            msWrite2ByteMask(REG_0030D6, usCommandAddress, MASKBIT(15:0)); // REG_MHL_CBUS1_6B[15:0], Xdata_write_address.
            msWriteByteMask(REG_0030D9, BIT7, BIT7); // REG_MHL_CBUS1_6C[15], reset xdata write address
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(bFirstBlock)
            {
                usCommandAddress = MHL_RX_XRAM_ADDR_FIRST_PORT2;
            }
            else
            {
                usCommandAddress = MHL_RX_XRAM_ADDR_SECOND_PORT2;
            }

            msWrite2ByteMask(REG_0032D6, usCommandAddress, MASKBIT(15:0)); // REG_MHL_CBUS2_6B[15:0], Xdata_write_address.
            msWriteByteMask(REG_0032D9, BIT7, BIT7); // REG_MHL_CBUS2_6C[15], reset xdata write address
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(bFirstBlock)
            {
                usCommandAddress = MHL_RX_XRAM_ADDR_FIRST_PORT3;
            }
            else
            {
                usCommandAddress = MHL_RX_XRAM_ADDR_SECOND_PORT3;
            }

            msWrite2ByteMask(REG_0034D6, usCommandAddress, MASKBIT(15:0)); // REG_MHL_CBUS3_6B[15:0], Xdata_write_address.
            msWriteByteMask(REG_0034D9, BIT7, BIT7); // REG_MHL_CBUS3_6C[15], reset xdata write address
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(bFirstBlock)
            {
                usCommandAddress = MHL_RX_XRAM_ADDR_FIRST_PORT4;
            }
            else
            {
                usCommandAddress = MHL_RX_XRAM_ADDR_SECOND_PORT4;
            }

            msWrite2ByteMask(REG_0036D6, usCommandAddress, MASKBIT(15:0)); // REG_MHL_CBUS4_6B[15:0], Xdata_write_address.
            msWriteByteMask(REG_0036D9, BIT7, BIT7); // REG_MHL_CBUS4_6C[15], reset xdata write address
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(bFirstBlock)
            {
                usCommandAddress = MHL_RX_XRAM_ADDR_FIRST_PORT5;
            }
            else
            {
                usCommandAddress = MHL_RX_XRAM_ADDR_SECOND_PORT5;
            }

            msWrite2ByteMask(REG_0038D6, usCommandAddress, MASKBIT(15:0)); // REG_MHL_CBUS5_6B[15:0], Xdata_write_address.
            msWriteByteMask(REG_0038D9, BIT7, BIT7); // REG_MHL_CBUS5_6C[15], reset xdata write address
            
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetRxXdataValue()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
BYTE mhal_mhl_GetRxXdataValue(BYTE ucPortSelect, BYTE ucCommandAddress)//0~128	 alan.lee
{        
	BYTE ucXdataValue = 0;
    WORD usCbusXdataAddress = 0;
	
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
		    usCbusXdataAddress = MHL_CBUS_SRAM_ADDRESS_PORT0;
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
		    usCbusXdataAddress = MHL_CBUS_SRAM_ADDRESS_PORT1;
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
		    usCbusXdataAddress = MHL_CBUS_SRAM_ADDRESS_PORT2;
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
		    usCbusXdataAddress = MHL_CBUS_SRAM_ADDRESS_PORT3;
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
		    usCbusXdataAddress = MHL_CBUS_SRAM_ADDRESS_PORT4;
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
		    usCbusXdataAddress = MHL_CBUS_SRAM_ADDRESS_PORT5;
            break;
#endif

        default:

            break;
    };

    ucXdataValue = R2_XSRAM_READ(usCbusXdataAddress + ucCommandAddress);

    return ucXdataValue;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetCbusReceiveFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_mhl_GetCbusReceiveFlag(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            bFlag = ((msReadByte(REG_002E74) & BIT3) ? TRUE : FALSE); // REG_MHL_CBUS0_3A[3], Receive packet valid interrupt.

            if(bFlag)
            {
                msWriteByteMask(REG_002E74, BIT0, BIT0); // REG_MHL_CBUS0_3A[0]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            bFlag = ((msReadByte(REG_003074) & BIT3) ? TRUE : FALSE); // REG_MHL_CBUS1_3A[3], Receive packet valid interrupt.

            if(bFlag)
            {
                msWriteByteMask(REG_003074, BIT0, BIT0); // REG_MHL_CBUS1_3A[0]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            bFlag = ((msReadByte(REG_003274) & BIT3) ? TRUE : FALSE); // REG_MHL_CBUS2_3A[3], Receive packet valid interrupt.

            if(bFlag)
            {
                msWriteByteMask(REG_003274, BIT0, BIT0); // REG_MHL_CBUS2_3A[0]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            bFlag = ((msReadByte(REG_003474) & BIT3) ? TRUE : FALSE); // REG_MHL_CBUS3_3A[3], Receive packet valid interrupt.

            if(bFlag)
            {
                msWriteByteMask(REG_003474, BIT0, BIT0); // REG_MHL_CBUS3_3A[0]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            bFlag = ((msReadByte(REG_003674) & BIT3) ? TRUE : FALSE); // REG_MHL_CBUS4_3A[3], Receive packet valid interrupt.

            if(bFlag)
            {
                msWriteByteMask(REG_003674, BIT0, BIT0); // REG_MHL_CBUS4_3A[0]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            bFlag = ((msReadByte(REG_003874) & BIT3) ? TRUE : FALSE); // REG_MHL_CBUS5_3A[3], Receive packet valid interrupt.

            if(bFlag)
            {
                msWriteByteMask(REG_003874, BIT0, BIT0); // REG_MHL_CBUS5_3A[0]
            }
            
            break;
#endif

        default:

            break;
    };

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetStucktoLowFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_mhl_GetStucktoLowFlag(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;
    
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            bFlag = ((MEM_MSREAD_BYTE(REG_002F30) &BIT3) ?TRUE: FALSE); // REG_PM_MHL_CBUS0_18[3], Cbus stuck to low interrupt.

            if(bFlag)
            {
                msWriteByteMask(REG_002F30, BIT0, BIT0); // REG_PM_MHL_CBUS0_18[0]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            bFlag = ((MEM_MSREAD_BYTE(REG_003130) &BIT3) ?TRUE: FALSE); // REG_PM_MHL_CBUS1_18[3], Cbus stuck to low interrupt.

            if(bFlag)
            {
                msWriteByteMask(REG_003130, BIT0, BIT0); // REG_PM_MHL_CBUS1_18[0]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            bFlag = ((MEM_MSREAD_BYTE(REG_003330) &BIT3) ?TRUE: FALSE); // REG_PM_MHL_CBUS2_18[3], Cbus stuck to low interrupt.

            if(bFlag)
            {
                msWriteByteMask(REG_003330, BIT0, BIT0); // REG_PM_MHL_CBUS2_18[0]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            bFlag = ((MEM_MSREAD_BYTE(REG_003530) &BIT3) ?TRUE: FALSE); // REG_PM_MHL_CBUS3_18[3], Cbus stuck to low interrupt.

            if(bFlag)
            {
                msWriteByteMask(REG_003530, BIT0, BIT0); // REG_PM_MHL_CBUS3_18[0]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            bFlag = ((MEM_MSREAD_BYTE(REG_003730) &BIT3) ?TRUE: FALSE); // REG_PM_MHL_CBUS4_18[3], Cbus stuck to low interrupt.

            if(bFlag)
            {
                msWriteByteMask(REG_003730, BIT0, BIT0); // REG_PM_MHL_CBUS4_18[0]
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            bFlag = ((MEM_MSREAD_BYTE(REG_003930) &BIT3) ?TRUE: FALSE); // REG_PM_MHL_CBUS5_18[3], Cbus stuck to low interrupt.

            if(bFlag)
            {
                msWriteByteMask(REG_003930, BIT0, BIT0); // REG_PM_MHL_CBUS5_18[0]
            }
            
            break;
#endif

        default:

            break;
    };

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetECbusStateChangeFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_mhl_GetECbusStateChangeFlag(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;
    
    ucPortSelect = 0;
    
    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetEMSCReceiveFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
Bool mhal_mhl_GetEMSCReceiveFlag(BYTE ucPortSelect)
{
    Bool bFlag = FALSE;
    
    ucPortSelect = 0;

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetEMSCSendStatus()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
BYTE mhal_mhl_GetEMSCSendStatus(BYTE ucPortSelect)
{
    BYTE ucSendStatus = MHL_EMSC_SEND_NONE;
    
    ucPortSelect = 0;

    return ucSendStatus;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusForceToStandby()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_CbusForceToStandby(BYTE ucPortSelect)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_002F2F, BIT7, BIT7); // REG_PM_MHL_CBUS0_17[15]
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_00312F, BIT7, BIT7); // REG_PM_MHL_CBUS1_17[15]
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            msWriteByteMask(REG_00332F, BIT7, BIT7); // REG_PM_MHL_CBUS2_17[15]
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_00352F, BIT7, BIT7); // REG_PM_MHL_CBUS3_17[15]
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            msWriteByteMask(REG_00372F, BIT7, BIT7); // REG_PM_MHL_CBUS4_17[15]
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            msWriteByteMask(REG_00392F, BIT7, BIT7); // REG_PM_MHL_CBUS5_17[15]
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_ClockModeSwitchProc()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//
//**************************************************************************
void mhal_mhl_ClockModeSwitchProc(BYTE ucPortSelect, Bool bPacketPixelMode)
{
    if(bPacketPixelMode)
    {
        // Load packet pixel mode table
        _mhal_mhl_PacketPixelModeSetting(ucPortSelect);
    }
    else
    {
        // Load 24 bit table
        _mhal_mhl_24BitModeSetting(ucPortSelect);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CablePlugProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_CablePlugProc(BYTE ucPortSelect)
{
    // Set HPD
    _mhal_mhl_SetHPDControl(ucPortSelect, TRUE);
    // Cbus enable
    _mhal_mhl_CbusPadEnable(ucPortSelect, TRUE);
    // Set cable detect
    _mhal_mhl_SetCableDetect(ucPortSelect, TRUE);
    // Vbus charge
    _mhal_mhl_SetVbusCharge(ucPortSelect, TRUE);
    // Open common mode resistor
    _mhal_mhl_OpenCommonModeResistor(ucPortSelect, TRUE);
    // Rterm off
    mhal_mhl_ComboRtermControl(ucPortSelect, RX_RTERM_OFF);
    // Remove Cbus pull down 100K
    _mhal_mhl_SetCbusPullDown100K(ucPortSelect, FALSE);
    // Load 24 bit table
    _mhal_mhl_24BitModeSetting(ucPortSelect);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CableRemoveProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_CableRemoveProc(BYTE ucPortSelect)
{
    // Vbus discharge
    _mhal_mhl_SetVbusCharge(ucPortSelect, FALSE);
    // Cbus disable
    _mhal_mhl_CbusPadEnable(ucPortSelect, FALSE);
    // Clear cable detect
    _mhal_mhl_SetCableDetect(ucPortSelect, FALSE);
    // Short common mode resistor
    _mhal_mhl_OpenCommonModeResistor(ucPortSelect, TRUE);
    // Rterm HDMI
    mhal_mhl_ComboRtermControl(ucPortSelect, RX_HDMI_RTERM);
    // Set Cbus pull down 100K
    _mhal_mhl_SetCbusPullDown100K(ucPortSelect, TRUE);
    // Load HDMI table
    _mhal_mhl_HDMIByPassSetting(ucPortSelect);
    // Disable stuck to low interrupt
    _mhal_mhl_SetStucktoLowInterrupt(ucPortSelect, FALSE);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusConnectProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_CbusConnectProc(BYTE ucPortSelect)
{
    // Rterm MHL
    mhal_mhl_ComboRtermControl(ucPortSelect, RX_MHL_RTERM);
    // Short common mode resistor
    _mhal_mhl_OpenCommonModeResistor(ucPortSelect, FALSE);
    // Enable stuck to low interrupt
    _mhal_mhl_SetStucktoLowInterrupt(ucPortSelect, TRUE);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusStucktoLowProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_CbusStucktoLowProc(BYTE ucPortSelect)
{
    // Rterm off
    mhal_mhl_ComboRtermControl(ucPortSelect, RX_RTERM_OFF);
    // Open common mode resistor
    _mhal_mhl_OpenCommonModeResistor(ucPortSelect, TRUE);
    // Disable stuck to low interrupt
    _mhal_mhl_SetStucktoLowInterrupt(ucPortSelect, FALSE);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SourceChangeProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_SourceChangeProc(BYTE ucDisplayIndex)
{
    if(ucDisplayIndex == 0)
    {
        // Do nothing
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_PowerDownProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_PowerDownProc(BYTE ucPortSelect)
{
    // Cbus disable
    _mhal_mhl_CbusPadEnable(ucPortSelect, FALSE);
    // Clear cable detect
    _mhal_mhl_SetCableDetect(ucPortSelect, FALSE);
    
    if(mhal_mhl_CableDetect(ucPortSelect))
    {
        // Remove pad PU1K
        _mhal_mhl_SetPadPullUp1K(ucPortSelect, FALSE);
        // Enable Rterm HW control
        mhal_mhl_RtermControlHWMode(ucPortSelect, TRUE);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_PMStatePolling()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_PMStatePolling(BYTE ucPortSelect)
{
    Bool bCableDetect = mhal_mhl_CableDetect(ucPortSelect);

    // Vbus charge
    _mhal_mhl_SetVbusCharge(ucPortSelect, bCableDetect);
    // Set cable detect
    _mhal_mhl_SetCableDetect(ucPortSelect, bCableDetect);
    // Rterm HW control
    mhal_mhl_RtermControlHWMode(ucPortSelect, bCableDetect);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CableDetectEnableControl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_CableDetectEnableControl(BYTE ucPortSelect, Bool bCableDetectEnable)
{
    switch(ucPortSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORT0)
        case MHL_INPUT_SELECT_PORT0:
            if(bCableDetectEnable)
            {
                SET_MHL_CABLE_DETECT_ENABLE0();
            }
            else
            {
                CLR_MHL_CABLE_DETECT_ENABLE0();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
        case MHL_INPUT_SELECT_PORT1:
            if(bCableDetectEnable)
            {
                SET_MHL_CABLE_DETECT_ENABLE1();
            }
            else
            {
                CLR_MHL_CABLE_DETECT_ENABLE1();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
        case MHL_INPUT_SELECT_PORT2:
            if(bCableDetectEnable)
            {
                SET_MHL_CABLE_DETECT_ENABLE2();
            }
            else
            {
                CLR_MHL_CABLE_DETECT_ENABLE2();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
        case MHL_INPUT_SELECT_PORT3:
            if(bCableDetectEnable)
            {
                SET_MHL_CABLE_DETECT_ENABLE3();
            }
            else
            {
                CLR_MHL_CABLE_DETECT_ENABLE3();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
        case MHL_INPUT_SELECT_PORT4:
            if(bCableDetectEnable)
            {
                SET_MHL_CABLE_DETECT_ENABLE4();
            }
            else
            {
                CLR_MHL_CABLE_DETECT_ENABLE4();
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
        case MHL_INPUT_SELECT_PORT5:
            if(bCableDetectEnable)
            {
                SET_MHL_CABLE_DETECT_ENABLE5();
            }
            else
            {
                CLR_MHL_CABLE_DETECT_ENABLE5();
            }
            
            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CheckSignalStable()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_mhl_CheckSignalStable(BYTE ucPortSelect)
{
    return bSignalStable[ucPortSelect];
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetECbusStatusFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mhal_mhl_GetECbusStatusFlag(BYTE ucPortSelect)
{
    BYTE ucECbusStatusFlag = 0;
    
    ucPortSelect = 0;

    return ucECbusStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_ECbusStateChangeProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_ECbusStateChangeProc(BYTE ucPortSelect)
{
    ucPortSelect = 0;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SetMainLinkRate()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_SetMainLinkRate(BYTE ucPortSelect, BYTE ucLinkRate)
{
    switch(ucPortSelect)
    {
        case MHL_INPUT_SELECT_PORT0:
            
            break;

        case MHL_INPUT_SELECT_PORT1:
            
            break;

        case MHL_INPUT_SELECT_PORT2:
        case MHL_INPUT_SELECT_PORT3:
            if(ucLinkRate == MHL_AV_LINK_600)
            {

            }
            else if(ucLinkRate == MHL_AV_LINK_300)
            {

            }
            else
            {

            }
            
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_ECbusEventProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_ECbusEventProc(BYTE ucPortSelect, BYTE ucECbusEvent)
{
    ucPortSelect = ucECbusEvent;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetEMSCReceiveData()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_GetEMSCReceiveData(BYTE ucPortSelect, BYTE *bReveiceEMSC)
{
    if(bReveiceEMSC != NULL)
    {
        ucPortSelect = 0;
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_InsertEMSCSendData()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_InsertEMSCSendData(BYTE ucPortSelect, BYTE ucLength, BYTE *bSendEMSC)
{
    if((ucLength > 0) && (bSendEMSC != NULL))
    {
        ucPortSelect = 0;
    }
}
//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CablePlugProcByInit
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_CablePlugProcByInit(BYTE ucInputPort)
{    
    _mhal_mhl_SetCbusPullDown100K(ucInputPort, FALSE);
}

#endif // _MHAL_MHL_C_

