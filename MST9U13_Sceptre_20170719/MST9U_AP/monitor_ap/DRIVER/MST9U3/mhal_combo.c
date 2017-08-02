///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mhal_combo.c
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_COMBO_C_
#define _MHAL_COMBO_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "Global.h"
#include "mapi_combo.h"
#include "mdrv_combo.h"
#include "mhal_combo.h"
#include "msEread.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MHALCOMBO_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && MHALCOMBO_DEBUG
#define MHALCOMBO_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define MHALCOMBO_PRINT(format, ...)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
BYTE ucComboVersionIndex = 0;
WORD usComboIDIndex = 0;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  _mhal_combo_bank_offset()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
WORD _mhal_combo_bank_offset(BYTE ucPortSelect)
{
    WORD wOffset = 0;

    switch(ucPortSelect)
    {
        case COMBO_INPUT_SELECT_PORT0:
		    wOffset = 0x0000;
			break;
            
        case COMBO_INPUT_SELECT_PORT1:
		    wOffset = 0x0600;
			break;
            
        case COMBO_INPUT_SELECT_PORT2:
		    wOffset = 0x0E00;
			break;
            
        case COMBO_INPUT_SELECT_PORT3:
		    wOffset = 0x1600;
			break;
            
        case COMBO_INPUT_SELECT_PORT4:
		    wOffset = 0x1E00;
			break;

        default:
            
            break;
    };

	return wOffset;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_combo_phy_offset()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
WORD _mhal_combo_phy_offset(BYTE ucPortSelect)
{
    WORD wOffset = 0;

    switch(ucPortSelect)
    {
        case COMBO_INPUT_SELECT_PORT0:
		    wOffset = 0x0000;
			break;
            
        case COMBO_INPUT_SELECT_PORT1:
		    wOffset = 0x0100;
			break;
            
        case COMBO_INPUT_SELECT_PORT2:
		    wOffset = 0x0200;
			break;
            
        case COMBO_INPUT_SELECT_PORT3:
		    wOffset = 0x0300;
			break;
            
        case COMBO_INPUT_SELECT_PORT4:
		    wOffset = 0x0400;
			break;
            
        default:
            
            break;
    };

	return wOffset;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_combo_InsertEDIDData()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_combo_InsertEDIDData(BYTE ucAddress, BYTE ucData)
{
    msWriteByte(REG_003E4B, ucAddress); // REG_DDC_BANK_25[14:8]: DDC address port for CPU read/write
    msWriteByte(REG_003E4C, ucData); // REG_DDC_BANK_26[7:0]: DDC Data Port for cpu write
    msWriteByteMask(REG_003E43, BIT5, BIT5); // REG_DDC_BANK_21[13]: ADC sram write data pulse gen when cpu write
    
    while(msReadByte(REG_003E7B) & BIT5); // REG_DDC_BANK_3D[13]: ADC_0 cpu wr busy status (wr for ADC sram)
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_combo_EDIDWriteProtectEnable()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_combo_EDIDWriteProtectEnable(BYTE ucPortSelect, Bool bWriteProtect)
{
    switch(ucPortSelect)
    {
        case COMBO_INPUT_SELECT_VGA:
            msWriteByteMask(REG_003E49, bWriteProtect? BIT5: 0, BIT5); // REG_DDC_BANK_24[13]: A0 I2C write protect
            break;
            
        case COMBO_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_003E45, bWriteProtect? BIT5: 0, BIT5); // REG_DDC_BANK_22[13]: D0 I2C write protect
            break;

        case COMBO_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_003E4D, bWriteProtect? BIT5: 0, BIT5); // REG_DDC_BANK_26[13]: D1 I2C write protect
            break;

        case COMBO_INPUT_SELECT_PORT2:
            //msWriteByteMask(REG_003E58, bWriteProtect? BIT5: 0, BIT5); // REG_DDC_BANK_2C[5]: D2 I2C write protect
            msWriteByteMask(REG_003EA8, bWriteProtect? BIT5: 0, BIT5); // REG_DDC_BANK_54[5]: D3 I2C write protect
            break;

        case COMBO_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_003E58, bWriteProtect? BIT5: 0, BIT5); // REG_DDC_BANK_2C[5]: D2 I2C write protect
            //msWriteByteMask(REG_003EA8, bWriteProtect? BIT5: 0, BIT5); // REG_DDC_BANK_54[5]: D3 I2C write protect
            break;

        case COMBO_INPUT_SELECT_PORT4:
            //msWriteByteMask(REG_003E45, bWriteProtect? BIT5: 0, BIT5); // REG_DDC_BANK_22[13]: D0 I2C write protect
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_combo_InternalEDIDEnable()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_combo_InternalEDIDEnable(BYTE ucPortSelect, Bool bDDCEnable)
{
    switch(ucPortSelect)
    {
        case COMBO_INPUT_SELECT_VGA:
            msWriteByteMask(REG_003E49, bDDCEnable? BIT7: 0, BIT7); // REG_DDC_BANK_24[15]: A0 DDC enable
            break;
            
        case COMBO_INPUT_SELECT_PORT0:
            msWriteByteMask(REG_003E45, bDDCEnable? BIT7: 0, BIT7); // REG_DDC_BANK_22[15]: D0 DDC enable
            break;

        case COMBO_INPUT_SELECT_PORT1:
            msWriteByteMask(REG_003E4D, bDDCEnable? BIT7: 0, BIT7); // REG_DDC_BANK_26[15]: D1 DDC enable
            break;

        case COMBO_INPUT_SELECT_PORT2:
            //msWriteByteMask(REG_003E58, bDDCEnable? BIT7: 0, BIT7); // REG_DDC_BANK_2C[7]: D2 DDC enable
            msWriteByteMask(REG_003EA8, bDDCEnable? BIT7: 0, BIT7); // REG_DDC_BANK_54[7]: D3 DDC enable
            break;

        case COMBO_INPUT_SELECT_PORT3:
            msWriteByteMask(REG_003E58, bDDCEnable? BIT7: 0, BIT7); // REG_DDC_BANK_2C[7]: D2 DDC enable
            //msWriteByteMask(REG_003EA8, bDDCEnable? BIT7: 0, BIT7); // REG_DDC_BANK_54[7]: D3 DDC enable
            break;

        case COMBO_INPUT_SELECT_PORT4:
            //msWriteByteMask(REG_003E45, bDDCEnable? BIT7: 0, BIT7); // REG_DDC_BANK_22[15]: D0 DDC enable
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_LoadEDID()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_LoadEDID(BYTE ucPortSelect, Bool bDPInput, BYTE ucEDIDSize, BYTE ucEDIDAddr, BYTE *pComboEDID)
{
    BYTE uctemp = 0;
    BYTE ucSizeCount = 0;

    if((ucEDIDSize != 0) && (pComboEDID != NULL))
    {
        if(ucPortSelect == COMBO_INPUT_SELECT_VGA)
        {
            msWriteByteMask(REG_003EEB, 0, MASKBIT(4:0)); // REG_DDC_BANK_75[12:8]: Select EDID sram base address for cpu read/write

            for(uctemp = 0; uctemp < COMBO_EDID_BLOCK_SIZE; uctemp++)
            {
                _mhal_combo_InsertEDIDData(uctemp, pComboEDID[uctemp]);
            }

            msWriteByteMask(REG_003EFA, 0, MASKBIT(4:0)); // REG_DDC_BANK_7D[4:0]: Select A0 EDID sram base address
        }
        else
        {
            for(ucSizeCount = 0; ucSizeCount < ucEDIDSize; ucSizeCount++)
            {
                msWriteByteMask(REG_003EEB, ucEDIDAddr +ucSizeCount, MASKBIT(4:0)); // REG_DDC_BANK_75[12:8]: Select EDID sram base address for cpu read/write

                for(uctemp = 0; uctemp < COMBO_EDID_BLOCK_SIZE; uctemp++)
                {
                    _mhal_combo_InsertEDIDData(uctemp, pComboEDID[uctemp +ucSizeCount *COMBO_EDID_BLOCK_SIZE]);
                }
            }

            switch(ucPortSelect)
            {
                case COMBO_INPUT_SELECT_PORT0:
                    msWriteByteMask(REG_003EEC, ucEDIDAddr, MASKBIT(4:0)); // REG_DDC_BANK_76[4:0]: Select D0 EDID sram base address
        			break;

                case COMBO_INPUT_SELECT_PORT1:
                    msWriteByteMask(REG_003EED, ucEDIDAddr, MASKBIT(4:0)); // REG_DDC_BANK_76[12:8]: Select D1 EDID sram base address
        			break;

                case COMBO_INPUT_SELECT_PORT2:
                    //msWriteByteMask(REG_003EF6, ucEDIDAddr, MASKBIT(4:0)); // REG_DDC_BANK_7B[4:0]: Select D2 EDID sram base address
                    msWriteByteMask(REG_003EF7, ucEDIDAddr, MASKBIT(4:0)); // REG_DDC_BANK_7B[12:8]: Select D3 EDID sram base address
        			break;

                case COMBO_INPUT_SELECT_PORT3:
                    msWriteByteMask(REG_003EF6, ucEDIDAddr, MASKBIT(4:0)); // REG_DDC_BANK_7B[4:0]: Select D2 EDID sram base address
                    //msWriteByteMask(REG_003EF7, ucEDIDAddr, MASKBIT(4:0)); // REG_DDC_BANK_7B[12:8]: Select D3 EDID sram base address
        			break;

                case COMBO_INPUT_SELECT_PORT4:
                    msWriteByteMask(REG_003EF8, ucEDIDAddr, MASKBIT(4:0)); // REG_DDC_BANK_7C[4:0]: Select DP_AUX EDID sram base address
        			break;

                default:

                    break;
            };
        }

        if(!bDPInput)
        {
            _mhal_combo_InternalEDIDEnable(ucPortSelect, TRUE);
        }

        _mhal_combo_EDIDWriteProtectEnable(ucPortSelect, TRUE);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_LoadHDCPKey()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_LoadHDCPKey(BYTE ucPortSelect, Bool bDPInput, BYTE ucBcaps, BYTE *pHDCPKey, BYTE *pBKSV)
{
    static Bool bKeyFlag = FALSE;
    static Bool bP2LoadKey = FALSE;
    WORD ustemp = 0;
    
    // Load HDCP key
    if(!bKeyFlag)
    {
        if(pHDCPKey != NULL)
        {
#if(COMBO_LOAD_HDCP_USE_RIU)
            msWriteByteMask(REG_163080, 0x0C, 0x0C); //enable CPU write SRAM
            msWriteByteMask(REG_163082, 0x00, 0xFF); //key address
            msWriteByteMask(REG_163083, 0x00, 0x03); //key address
            msWriteByteMask(REG_163080, 0x10, 0x10); //load address

            for(ustemp = 0; ustemp < COMBO_HDCP_BKSV_SIZE; ustemp++)
            {
                msWriteByteMask(REG_163084, pBKSV[ustemp], 0xFF); // bksv
                msWriteByteMask(REG_163080, 0x10, 0x10); // write data pulse
                
                while(msReadByte(REG_16042E + wOffset) & 0x10);
            }

            for(ustemp = 0; ustemp < COMBO_HDCP_KEY_SIZE; ustemp++)
            {
                msWriteByteMask(REG_163084, pHDCPKey[ustemp], 0xFF); // bksv
                msWriteByteMask(REG_163080, 0x10, 0x10); // write data pulse
                
                while(msReadByte(REG_16042E + wOffset) & 0x10);
            }

            msWriteByteMask(REG_163080, 0x00, 0x0C); //disable CPU write
            
#else //XIU
            msWriteByteMask(REG_163080, MASKBIT(3:2)| BIT0, MASKBIT(3:2)| BIT0); // REG_COMBO_GP_40[0][2][3]: enable CPU write SRAM by XIU
            
            //msWrite2ByteMask(REG_162E00, 0, MASKBIT(9:0)); // REG_COMBO_HDCP_00[9:0]: address
            
            for(ustemp = 0; ustemp < COMBO_HDCP_BKSV_SIZE; ustemp++)
            {
                msWriteByteMask(REG_162E02, pBKSV[ustemp], 0xFF); // REG_COMBO_HDCP_01[7:0]: the data for CPU write into HDCP KEY SRAM through XIU
            }
            
            for(ustemp = 0; ustemp < COMBO_HDCP_KEY_SIZE; ustemp++)
            {
                msWriteByteMask(REG_162E02, pHDCPKey[ustemp], 0xFF); // REG_COMBO_HDCP_01[7:0]: the data for CPU write into HDCP KEY SRAM through XIU
            }
            
            msWriteByteMask(REG_163080, 0x00, MASKBIT(3:2) |BIT0); // REG_COMBO_GP_40[0][2][3]: disable CPU write SRAM by XIU
            
#endif

            bKeyFlag = TRUE;
        }
    }

    // Load Bksv and Bcaps
    if(pBKSV != NULL)
    {
        switch(ucPortSelect)
        {
            case COMBO_INPUT_SELECT_PORT0:
                msWriteByteMask(REG_16042F, BIT2, BIT2); // REG_COMBO_DVI_HDCP_P0_17[10]: HDCP enable for DDC
                // 74 write enable
                msWriteByteMask(REG_160433, BIT7, MASKBIT(7:6)); // REG_COMBO_DVI_HDCP_P0_19[14][15]
                
                for(ustemp = 0; ustemp < COMBO_HDCP_BKSV_SIZE; ustemp++)
                {
                    // 74 address: 0x00
                    msWrite2ByteMask(REG_16042E, ustemp, MASKBIT(9:0)); // REG_COMBO_DVI_HDCP_P0_17[9:0]: CPU r/w address (for hdcp_key_sram/74reg)
                    // bksv
                    msWriteByte(REG_160430, pBKSV[ustemp]); // REG_COMBO_DVI_HDCP_P0_18[7:0]:CPU write data port (for hdcp_key_sram/74reg)                  
                    msWriteByteMask(REG_160432, BIT5, BIT5); // REG_COMBO_DVI_HDCP_P0_19[5]: HDCP address load pulse generate                   
            	    msWriteByteMask(REG_160432, BIT4, BIT4); // REG_COMBO_DVI_HDCP_P0_19[4]: HDCP data write port pulse generate            		  
                }
                // 74 address: 0x40
                msWrite2ByteMask(REG_16042E, 0x40, MASKBIT(9:0)); // REG_COMBO_DVI_HDCP_P0_17[9:0]: CPU r/w address (for hdcp_key_sram/74reg)
                // bcaps
                msWriteByte(REG_160430, ucBcaps); // REG_COMBO_DVI_HDCP_P0_18[7:0]:CPU write data port (for hdcp_key_sram/74reg)                
                msWriteByteMask(REG_160432, BIT5, BIT5); // REG_COMBO_DVI_HDCP_P0_19[5]: HDCP address load pulse generate            
            	msWriteByteMask(REG_160432, BIT4, BIT4); // REG_COMBO_DVI_HDCP_P0_19[4]: HDCP data write port pulse generate            	
                
                // 74 write disable
                msWriteByteMask(REG_160433, 0, MASKBIT(7:6)); // REG_COMBO_DVI_HDCP_P0_19[14][15]
                //enable HDCP   
                msWrite2Byte(REG_160400, 0x0721); // REG_COMBO_DVI_HDCP_P0_00[5]: enable HDCP   
    			break;

            case COMBO_INPUT_SELECT_PORT1:
                msWriteByteMask(REG_16092F, BIT2, BIT2); // REG_COMBO_DVI_HDCP_P1_17[10]: HDCP enable for DDC
                // 74 write enable
                msWriteByteMask(REG_160933, BIT7, MASKBIT(7:6)); // REG_COMBO_DVI_HDCP_P1_19[14][15]
                
                for(ustemp = 0; ustemp < COMBO_HDCP_BKSV_SIZE; ustemp++)
                {
                    // 74 address: 0x00
                    msWrite2ByteMask(REG_16092E, ustemp, MASKBIT(9:0)); // REG_COMBO_DVI_HDCP_P1_17[9:0]: CPU r/w address (for hdcp_key_sram/74reg)
                    // bksv
                    msWriteByte(REG_160930, pBKSV[ustemp]); // REG_COMBO_DVI_HDCP_P1_18[7:0]:CPU write data port (for hdcp_key_sram/74reg)                  
                    msWriteByteMask(REG_160932, BIT5, BIT5); // REG_COMBO_DVI_HDCP_P1_19[5]: HDCP address load pulse generate                   
            	    msWriteByteMask(REG_160932, BIT4, BIT4); // REG_COMBO_DVI_HDCP_P1_19[4]: HDCP data write port pulse generate            		  
                }
                // 74 address: 0x40
                msWrite2ByteMask(REG_16092E, 0x40, MASKBIT(9:0)); // REG_COMBO_DVI_HDCP_P1_17[9:0]: CPU r/w address (for hdcp_key_sram/74reg)
                // bcaps
                msWriteByte(REG_160930, ucBcaps); // REG_COMBO_DVI_HDCP_P1_18[7:0]:CPU write data port (for hdcp_key_sram/74reg)                
                msWriteByteMask(REG_160932, BIT5, BIT5); // REG_COMBO_DVI_HDCP_P1_19[5]: HDCP address load pulse generate            
            	msWriteByteMask(REG_160932, BIT4, BIT4); // REG_COMBO_DVI_HDCP_P1_19[4]: HDCP data write port pulse generate            	
                
                // 74 write disable
                msWriteByteMask(REG_160933, 0, MASKBIT(7:6)); // REG_COMBO_DVI_HDCP_P1_19[14][15]
                //enable HDCP   
                msWrite2Byte(REG_160900, 0x0721); // REG_COMBO_DVI_HDCP_P1_00[5]: enable HDCP   
    			break;

            case COMBO_INPUT_SELECT_PORT2:
            case COMBO_INPUT_SELECT_PORT3:  
                if(bP2LoadKey == FALSE)
                {
                    msWriteByteMask(REG_16102F, BIT2, BIT2); // REG_COMBO_DVI_HDCP_DUAL_17[10]: HDCP enable for DDC
                    // 74 write enable
                    msWriteByteMask(REG_161033, BIT7, MASKBIT(7:6)); // REG_COMBO_DVI_HDCP_DUAL_19[14][15]
                
                    for(ustemp = 0; ustemp < COMBO_HDCP_BKSV_SIZE; ustemp++)
                    {
                        // 74 address: 0x00
                        msWrite2ByteMask(REG_16102E, ustemp, MASKBIT(9:0)); // REG_COMBO_DVI_HDCP_DUAL_17[9:0]: CPU r/w address (for hdcp_key_sram/74reg)
                        // bksv
                        msWriteByte(REG_161030, pBKSV[ustemp]); // REG_COMBO_DVI_HDCP_DUAL_18[7:0]:CPU write data port (for hdcp_key_sram/74reg)                  
                        msWriteByteMask(REG_161032, BIT5, BIT5); // REG_COMBO_DVI_HDCP_DUAL_19[5]: HDCP address load pulse generate                   
            	        msWriteByteMask(REG_161032, BIT4, BIT4); // REG_COMBO_DVI_HDCP_DUAL_19[4]: HDCP data write port pulse generate            		  
                    }
                    // 74 address: 0x40
                    msWrite2ByteMask(REG_16102E, 0x40, MASKBIT(9:0)); // REG_COMBO_DVI_HDCP_DUAL_17[9:0]: CPU r/w address (for hdcp_key_sram/74reg)
                    // bcaps
                    msWriteByte(REG_161030, ucBcaps); // REG_COMBO_DVI_HDCP_DUAL_18[7:0]:CPU write data port (for hdcp_key_sram/74reg)                
                    msWriteByteMask(REG_161032, BIT5, BIT5); // REG_COMBO_DVI_HDCP_DUAL_19[5]: HDCP address load pulse generate            
            	    msWriteByteMask(REG_161032, BIT4, BIT4); // REG_COMBO_DVI_HDCP_DUAL_19[4]: HDCP data write port pulse generate            	
                
                    // 74 write disable
                    msWriteByteMask(REG_161033, 0, MASKBIT(7:6)); // REG_COMBO_DVI_HDCP_DUAL_19[14][15]
                    //enable HDCP   
                    msWrite2Byte(REG_161000, 0x0721); // REG_COMBO_DVI_HDCP_DUAL_00[5]: enable HDCP  
                    bP2LoadKey = TRUE;                
                }
                
                break;

            case COMBO_INPUT_SELECT_PORT4:
                if(bDPInput)
                {
            		msWriteByteMask(REG_163080, MASKBIT(3:2), MASKBIT(3:2)); // REG_COMBO_GP_40[2][3]:
                    
            		for(ustemp = 0; ustemp < COMBO_HDCP_BKSV_SIZE; ustemp++)
            		{
            			msWrite2ByteMask(REG_163082, ustemp, MASKBIT(9:0)); // REG_COMBO_GP_41[9:0]: CPU r/w address (for hdcp_key_sram/74reg)
            			msWriteByte(REG_163084, pBKSV[ustemp]); // REG_COMBO_GP_42[7:0]: CPU write data port (for hdcp_key_sram/74reg) 
                        // Aux PM 
            			msWriteByte(REG_0011A0+ustemp, pBKSV[ustemp]);
            			Delay1ms(5);
            			msWriteByteMask(REG_163080, BIT4, BIT4); // REG_COMBO_GP_40[4]: HDCP address load pulse generate
            			Delay1ms(5);
            			msWriteByteMask(REG_163080, BIT5, BIT5); // REG_COMBO_GP_40[5]: HDCP data read port pulse generate
            			Delay1ms(5);
            		}

                    msWriteByteMask(REG_163080, 0, MASKBIT(3:2)); // REG_COMBO_GP_40[2][3]:
                }
                
    			break;

            default:

                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_ClockEnable()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_ClockEnable(BYTE ucPortSelect, Bool bMainSwitch)
{
    if(bMainSwitch)
    {
        msWrite2Byte(REG_163006, 0xFFFF); // REG_COMBO_GP_TOP_03[15:0]: Misc clock enable
    	msWrite2Byte(REG_163008, 0x3FFF); // REG_COMBO_GP_TOP_04[13:0]: Misc clock enable

        msWrite2Byte(0x1630EE, 0x1FF); // REG_COMBO_GP_TOP_77[8:0]: Clk to scaler

        msEread_Init(&usComboIDIndex, &ucComboVersionIndex);
    }
    
    switch(ucPortSelect)
    {
        case COMBO_INPUT_SELECT_PORT0:
            msWrite2Byte(REG_163022, 0xFFFF); // REG_COMBO_GP_TOP_11[15:0]: Clock enable port0
			break;

        case COMBO_INPUT_SELECT_PORT1:
		    msWrite2Byte(REG_163042, 0xFFFF); // REG_COMBO_GP_TOP_21[15:0]: Clock enable port1
			break;

        case COMBO_INPUT_SELECT_PORT2:
        case COMBO_INPUT_SELECT_PORT3:
		    msWrite2Byte(REG_163062, 0xFFFF); // REG_COMBO_GP_TOP_31[15:0]: Clock enable port2
			break;

        default:

            break;
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_AudioInitial()
//  [Description]
//                  initial audio common settings
//  [Arguments]:
//                  none
//  [Return]:
//                  none
//**************************************************************************
void mhal_combo_AudioInitial(void)
{
    msWriteByteMask(REG_1630B2, 0x00, 0x3F); // REG_COMBO_GP_TOP_59[3:0]: i2s 0~7 out OEN, [4]:spdif out OEN, [5]: i2s out mute OEN

    //I2S setting	
    msWriteByteMask(REG_163002, 0x10, 0x10); // [4]: reg_i2s_mclk_192fs_sel: 192f2(MHL/HDMI only)
    msWriteByteMask(REG_1630A1, BIT2, BIT3|BIT2); // 01: 24 bits
    msWriteByteMask(REG_1630A4, 0, BIT2); // I2S-Justified
    msWriteByteMask(REG_16300B, BIT7, BIT7); //  [15]: i2s bck inv
    
    // Gain
    msWriteByte(REG_1630A5, 0xC0); // REG_COMBO_GP_TOP_52[15:8]: 0xC0 is 0 dB, default 0x00 is mute will cause i2s out data fade out

    // Formula:  ( 12Mhz * 2^km * fbdiv ) / (2^kn * 2^kp * ddiv)
    msWriteByte(REG_162F12, 0x22); // REG_COMBO_PLL_TOP_09[7:4]: reg_apll_km_reg, [3:0]:reg_apll_divkp_reg
    msWriteByteMask(REG_162F13, 0x00, 0x60); // REG_COMBO_PLL_TOP_09[14:13]: reg_apll_kn_reg
    msWriteByte(REG_162F15, 0x62); // REG_COMBO_PLL_TOP_0A[11:8]:reg_aupll_ddiv, [7:4]:reg_aupll_fbdiv
    msWriteByteMask(REG_162F07, 0x00, 0x01); // REG_COMBO_PLL_TOP_03[8]: turn off power down
    msWriteByte(REG_162F0E, 0x50); // REG_COMBO_PLL_TOP_07[15:0]: makes apll lock
    
    // Setting for HDMI CTS
    msWriteByteMask(REG_162F30, BIT7, BIT7); // REG_COMBO_PLL_TOP_18[7]: enable Audio HDMI CTS-N synthesizer, [0]: Select CTS[19:0].
    msWriteByteMask(REG_162F40, 0, MASKBIT(1:0)); // REG_COMBO_PLL_TOP_20[0]: APLL synthesizer DIN, FBIN input select, [1]: APLL input control and clock source select

    // Setting for DP only
    msWriteByteMask(REG_162F06, 0, BIT0); // REG_COMBO_PLL_TOP_03[0]: APLL input divider select from synthesizer
    
    // Fading setting
#if ENABLE_AUDIO_AUTO_FADING
    msWriteByteMask(REG_1630A0, 0x08, 0x0C); // REG_COMBO_GP_TOP_50[3:2]: reg_fading_mode, 10: automatic fading mode
#endif
    msWriteByteMask(REG_163093, 0x0C, 0x0C); // REG_COMBO_GP_TOP_49[11:10]: reg_fad_speed_sel, lfading speed select, 11: 8X
    msWrite2Byte(REG_16308C, 0xFFFF); // REG_COMBO_GP_TOP_46[15:0]: zero-crossing threshold
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_ClockRtermControl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_ClockRtermControl(BYTE ucPortSelect, Bool bRtermEnable)
{
    if(ucComboVersionIndex > 0)
    {
        switch(ucPortSelect)
        {
            case COMBO_INPUT_SELECT_PORT0:
                msWriteByteMask(REG_001724, bRtermEnable? 0: BIT7, BIT7); // REG_PM_BANK_12[7]: Power control P0 clock r-term
                msWriteByteMask(REG_112068, bRtermEnable? 0: BIT1, BIT1); // REG_COMBO_PHY0_34[1]: Power down standyby clock amplifier
                break;

            case COMBO_INPUT_SELECT_PORT1:
    		    msWriteByteMask(REG_001726, bRtermEnable? 0: BIT7, BIT7); // REG_PM_BANK_13[7]: Power control P1 clock r-term
                msWriteByteMask(REG_112268, bRtermEnable? 0: BIT1, BIT1); // REG_COMBO_PHY2_34[1]: Power down standyby clock amplifier
                break;

            case COMBO_INPUT_SELECT_PORT2:
    		    msWriteByteMask(REG_001720, bRtermEnable? 0: BIT3, BIT3); // REG_PM_BANK_10[3]: Power control P2 clock r-term
                msWriteByteMask(REG_1124B7, bRtermEnable? 0: BIT4, BIT4); // REG_COMBO_PHY4_5B[12]: Power down clock amplifier
                break;

            case COMBO_INPUT_SELECT_PORT3:
    		    msWriteByteMask(REG_001722, bRtermEnable? 0: BIT3, BIT3); // REG_PM_BANK_11[3]: Power control P3 clock r-term
                msWriteByteMask(REG_1124B7, bRtermEnable? 0: BIT4, BIT4); //  REG_COMBO_PHY4_5B[12]: Power down clock amplifier
                break;

            case COMBO_INPUT_SELECT_PORT4:
    		    //msWriteByteMask(REG_001781, bRtermEnable? 0: BIT4, BIT4); // REG_PM_BANK_40[12]: Power control P1 clock r-term
                break;

            default:

                break;
        };
    }
    else
    {
        switch(ucPortSelect)
        {
            case COMBO_INPUT_SELECT_PORT0:
                msWriteByteMask(REG_001724, bRtermEnable? 0: BIT7, BIT7); // REG_PM_BANK_12[7]: Power control P0 clock r-term
                msWriteByteMask(REG_112068, bRtermEnable? 0: BIT1, BIT1); // REG_COMBO_PHY0_34[1]: Power down standyby clock amplifier
                break;

            case COMBO_INPUT_SELECT_PORT1:
    		    msWriteByteMask(REG_001726, bRtermEnable? 0: BIT7, BIT7); // REG_PM_BANK_13[7]: Power control P1 clock r-term
                msWriteByteMask(REG_112268, bRtermEnable? 0: BIT1, BIT1); // REG_COMBO_PHY2_34[1]: Power down standyby clock amplifier
                break;

            case COMBO_INPUT_SELECT_PORT2:
    		    msWriteByteMask(REG_001720, bRtermEnable? 0: BIT0, BIT0); // REG_PM_BANK_10[0]: Power control P2 clock r-term
                msWriteByteMask(REG_1124B7, bRtermEnable? 0: BIT4, BIT4); // REG_COMBO_PHY4_5B[12]: Power down clock amplifier
                break;

            case COMBO_INPUT_SELECT_PORT3:
    		    msWriteByteMask(REG_001722, bRtermEnable? 0: BIT0, BIT0); // REG_PM_BANK_11[0]: Power control P3 clock r-term
                msWriteByteMask(REG_1124B7, bRtermEnable? 0: BIT4, BIT4); //  REG_COMBO_PHY4_5B[12]: Power down clock amplifier
                break;

            case COMBO_INPUT_SELECT_PORT4:
    		    //msWriteByteMask(REG_001781, bRtermEnable? 0: BIT4, BIT4); // REG_PM_BANK_40[12]: Power control P1 clock r-term
                break;

            default:

                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_DataRtermControl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_DataRtermControl(BYTE ucPortSelect, Bool bRtermEnable)
{
    if(ucComboVersionIndex > 0)
    {
        switch(ucPortSelect)
        {
            case COMBO_INPUT_SELECT_PORT0:
                msWriteByteMask(REG_112107, bRtermEnable? 0: MASKBIT(2:1), MASKBIT(2:1)); // REG_COMBO_PHY1_03[10:9], Power on port0 data rterm.
                msWriteByteMask(REG_001724, bRtermEnable? 0: BIT4, BIT4); // REG_PM_BANK_12[4]: Power control P0 data rterm
    			break;

            case COMBO_INPUT_SELECT_PORT1:
                msWriteByteMask(REG_112307, bRtermEnable? 0: MASKBIT(2:1), MASKBIT(2:1)); // REG_COMBO_PHY3_03[10:9], Power on port1 data rterm.
                msWriteByteMask(REG_001726, bRtermEnable? 0: BIT4, BIT4); // REG_PM_BANK_13[4]: Power control P1 data rterm
    			break;

            case COMBO_INPUT_SELECT_PORT2:
                msWriteByteMask(REG_001720, bRtermEnable? 0: MASKBIT(2:0), MASKBIT(2:0)); // REG_PM_BANK_10[2:0], Power control port2 data rterm.
    			break;

            case COMBO_INPUT_SELECT_PORT3:
                msWriteByteMask(REG_001722, bRtermEnable? 0: MASKBIT(2:0), MASKBIT(2:0)); // REG_PM_BANK_11[2:0], Power control port3 data rterm.
    			break;

            case COMBO_INPUT_SELECT_PORT4:
                //msWriteByteMask(REG_0017A2, bRtermEnable? 0: MASKBIT(7:4), MASKBIT(7:4)); // REG_PM_BANK_51[7:4], Power control port4 data rterm.
    			break;

            default:

                break;
        };
    }
    else
    {
        switch(ucPortSelect)
        {
            case COMBO_INPUT_SELECT_PORT0:
                msWriteByteMask(REG_112107, bRtermEnable? 0: MASKBIT(2:0), MASKBIT(2:0)); // REG_COMBO_PHY1_03[10:8], Power on port0 rterm.
                break;

            case COMBO_INPUT_SELECT_PORT1:
                msWriteByteMask(REG_112307, bRtermEnable? 0: MASKBIT(2:0), MASKBIT(2:0)); // REG_COMBO_PHY3_03[10:8], Power on port1 rterm.
                break;

            case COMBO_INPUT_SELECT_PORT2:
                msWriteByteMask(REG_001720, bRtermEnable? 0: MASKBIT(2:0), MASKBIT(2:0)); // REG_PM_BANK_10[2:0], Power control port2 data rterm.
    			break;

            case COMBO_INPUT_SELECT_PORT3:
                msWriteByteMask(REG_001722, bRtermEnable? 0: MASKBIT(2:0), MASKBIT(2:0)); // REG_PM_BANK_11[2:0], Power control port3 data rterm.
    			break;

            case COMBO_INPUT_SELECT_PORT4:
                //msWriteByteMask(REG_0017A2, bRtermEnable? 0: MASKBIT(7:4), MASKBIT(7:4)); // REG_PM_BANK_51[7:4], Power control port4 data rterm.
    			break;

            default:

                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_DoSWReset()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_DoSWReset(BYTE ucPortSelect, DWORD ulSWResetValue)
{
    WORD usSWResetValueL = ulSWResetValue;
    WORD usSWResetValueH = (ulSWResetValue >> 16);

    switch(ucPortSelect)
    {
        case COMBO_INPUT_SELECT_PORT0:
            if(usSWResetValueL > 0)
            {
                msWrite2Byte(REG_11201C, usSWResetValueL); // REG_COMBO_PHY0_0E[15:0]: SW reset
                msWrite2Byte(REG_11201C, 0); // REG_COMBO_PHY0_0E[15:0]: SW reset

				if(usSWResetValueL & BIT2)
				{
					msWrite2Byte(REG_16027C, BIT0); // REG_DVI_DTOP_P0_3E[0]: DVI SW reset
					msWrite2Byte(REG_16027C, 0); // REG_DVI_DTOP_P0_3E[0]: DVI SW reset
				}
				if(usSWResetValueL & BIT4)
				{
					msWrite2Byte(REG_16027C, BIT2); // REG_DVI_DTOP_P0_3E[2]: HDCP SW reset
					msWrite2Byte(REG_16027C, 0); // REG_DVI_DTOP_P0_3E[2]: HDCP SW reset
				}
				if(usSWResetValueL & BIT5)
				{
					msWrite2Byte(REG_163068, BIT3); // REG_COMBO_GP_TOP_34[3]: DVI2MIU SW reset
					msWrite2Byte(REG_163068, 0); // REG_COMBO_GP_TOP_34[3]: DVI2MIU SW reset
				}
				if(usSWResetValueL & BIT6)
				{
					msWrite2Byte(REG_16027C, BIT3); // REG_DVI_DTOP_P0_3E[3]: HDMI SW reset
					msWrite2Byte(REG_16027C, 0); // REG_DVI_DTOP_P0_3E[3]: HDMI SW reset
				}
            }

            if(usSWResetValueH > 0)
            {
                msWriteByteMask(REG_11201E, usSWResetValueH, MASKBIT(1:0)); // REG_COMBO_PHY0_0F[1:0]: SW reset
                msWriteByteMask(REG_11201E, 0, MASKBIT(1:0)); // REG_COMBO_PHY0_0F[1:0]: SW reset
            }

			break;

        case COMBO_INPUT_SELECT_PORT1:
            if(usSWResetValueL > 0)
            {
                msWrite2Byte(REG_11221C, usSWResetValueL); // REG_COMBO_PHY2_0E[15:0]: SW reset
                msWrite2Byte(REG_11221C, 0); // REG_COMBO_PHY2_0E[15:0]: SW reset

				if(usSWResetValueL & BIT2)
				{
					msWrite2Byte(REG_16077C, BIT0); // REG_DVI_DTOP_P1_3E[0]: DVI SW reset
					msWrite2Byte(REG_16077C, 0); // REG_DVI_DTOP_P1_3E[0]: DVI SW reset
				}
				if(usSWResetValueL & BIT4)
				{
					msWrite2Byte(REG_16077C, BIT2); // REG_DVI_DTOP_P1_3E[2]: HDCP SW reset
					msWrite2Byte(REG_16077C, 0); // REG_DVI_DTOP_P1_3E[2]: HDCP SW reset
				}
				if(usSWResetValueL & BIT5)
				{
					msWrite2Byte(REG_163068, BIT3); // REG_COMBO_GP_TOP_34[3]: DVI2MIU SW reset
					msWrite2Byte(REG_163068, 0); // REG_COMBO_GP_TOP_34[3]: DVI2MIU SW reset
				}
				if(usSWResetValueL & BIT6)
				{
					msWrite2Byte(REG_16077C, BIT3); // REG_DVI_DTOP_P1_3E[3]: HDMI SW reset
					msWrite2Byte(REG_16077C, 0); // REG_DVI_DTOP_P1_3E[3]: HDMI SW reset
				}
            }

            if(usSWResetValueH > 0)
            {
                msWriteByteMask(REG_11221E, usSWResetValueH, MASKBIT(1:0)); // REG_COMBO_PHY2_0F[1:0]: SW reset
                msWriteByteMask(REG_11221E, 0, MASKBIT(1:0)); // REG_COMBO_PHY2_0F[1:0]: SW reset
            }

			break;

        case COMBO_INPUT_SELECT_PORT2:
        case COMBO_INPUT_SELECT_PORT3:
            if(usSWResetValueL > 0)
            {
                msWrite2Byte(REG_11241C, usSWResetValueL); // REG_COMBO_PHY4_0E[15:0]: SW reset
                msWrite2Byte(REG_11241C, 0); // REG_COMBO_PHY4_0E[15:0]: SW reset

				if(usSWResetValueL & BIT2)
				{
					msWrite2Byte(REG_160C7C, BIT0); // REG_DVI_DTOP_DUAL_3E[0]: DVI SW reset
					msWrite2Byte(REG_160C7C, 0); // REG_DVI_DTOP_DUAL_3E[0]: DVI SW reset
				}
                if(usSWResetValueL & BIT4)
				{
					msWrite2Byte(REG_160C7C, BIT2); // REG_DVI_DTOP_DUAL_3E[2]: HDCP SW reset
					msWrite2Byte(REG_160C7C, 0); // REG_DVI_DTOP_DUAL_3E[2]: HDCP SW reset
				}
				if(usSWResetValueL & BIT5)
				{
					msWrite2Byte(REG_163068, BIT3); // REG_COMBO_GP_TOP_34[3]: DVI2MIU SW reset
					msWrite2Byte(REG_163068, 0); // REG_COMBO_GP_TOP_34[3]: DVI2MIU SW reset
				}
				if(usSWResetValueL & BIT6)
				{
					msWrite2Byte(REG_160C7C, BIT3); // REG_DVI_DTOP_DUAL_3E[3]: HDMI SW reset
					msWrite2Byte(REG_160C7C, 0); // REG_DVI_DTOP_DUAL_3E[3]: HDMI SW reset
				}
            }

            if(usSWResetValueH > 0)
            {
                msWriteByteMask(REG_11241E, usSWResetValueH, MASKBIT(1:0)); // REG_COMBO_PHY4_0F[1:0]: SW reset
                msWriteByteMask(REG_11241E, 0, MASKBIT(1:0)); // REG_COMBO_PHY4_0F[1:0]: SW reset
            }

			break;

        case COMBO_INPUT_SELECT_PORT4:
            if(usSWResetValueL > 0)
            {
                //msWrite2Byte(REG_162452, usSWResetValueL); // REG_COMBO_ATOP4_29[15:0]: SW reset
                //msWrite2Byte(REG_162452, 0); // REG_COMBO_ATOP4_29[15:0]: SW reset
            }

            if(usSWResetValueH > 0)
            {
                //msWriteByteMask(REG_162454, usSWResetValueH, MASKBIT(1:0)); // REG_COMBO_ATOP4_2A[1:0]: SW reset
                //msWriteByteMask(REG_162454, 0, MASKBIT(1:0)); // REG_COMBO_ATOP4_2A[1:0]: SW reset
            }

			break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_ConfigAudioSource()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_ConfigAudioSource(Bool bIsTMDS, BYTE ucPortSelect, BYTE ucHBR2Select)
{
    BYTE ucClockSelect = 0;
    BYTE ucSourceSelect = 0;
    
    switch(ucPortSelect)
    {
        case COMBO_INPUT_SELECT_PORT0:
            ucClockSelect = 0;
            ucSourceSelect = 0;
			break;

        case COMBO_INPUT_SELECT_PORT1:
            ucClockSelect = 1;
            ucSourceSelect = 1;
			break;

        case COMBO_INPUT_SELECT_PORT2:
        case COMBO_INPUT_SELECT_PORT3:
            ucClockSelect = 2;
            ucSourceSelect = 2;
			break;

        case COMBO_INPUT_SELECT_PORT4:
            ucClockSelect = 4;
            ucSourceSelect = 12;
			break;

        default:

            break;
    };
    
    // Audio clock select
    msWriteByteMask(REG_163004, (ucClockSelect <<4), MASKBIT(6:4)); // REG_COMBO_GP_TOP_02[6:4]: audio engine clock select

    // Audio source select
    msWriteByteMask(REG_163088, ucSourceSelect, 0x0F); // REG_COMBO_GP_TOP_44[3:0]: audio engine source select
    
    if(bIsTMDS)
    {
        ucHBR2Select = 0;
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_ConfigAPLLSynthesizer()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_ConfigAPLLSynthesizer(Bool bIsTMDS, BYTE ucPortSelect, BYTE ucHBR2Select)
{
    BYTE ucSourceSelect = 0;
    
    if(bIsTMDS == TRUE)
    {
        msWriteByteMask(REG_162F41, 0, BIT0); // REG_COMBO_PLL_TOP_20[8]: disable APLL VCO frequency x2
        msWriteByteMask(REG_162F40, 0, BIT0| BIT1); // REG_COMBO_PLL_TOP_20[1:0]: APLL synthesizer DIN and FBIN input select, APLL input control and clock source select
        msWriteByteMask(REG_162F22, 0, BIT5); // REG_COMBO_PLL_TOP_11[5]: Step override enable
    }
    else //DP
    {
        if(ucHBR2Select != COMBO_DP_HBR2_ENGINE_NONE) 
        {
            
        }
        
        msWriteByteMask(REG_162F40, BIT0| BIT1, BIT0| BIT1); // REG_COMBO_PLL_TOP_20[1:0]: APLL synthesizer DIN and FBIN input select, APLL input control and clock source select
    }

    switch(ucPortSelect)
    {
        case COMBO_INPUT_SELECT_PORT0:
            ucSourceSelect = 0;
			break;

        case COMBO_INPUT_SELECT_PORT1:
            ucSourceSelect = 1;
			break;

        case COMBO_INPUT_SELECT_PORT2:
        case COMBO_INPUT_SELECT_PORT3:
            ucSourceSelect = 2;
			break;

        case COMBO_INPUT_SELECT_PORT4:
            ucSourceSelect = 14;
			break;

        default:

            break;
    };

    msWriteByteMask(REG_162F40, (ucSourceSelect <<4), 0xF0); // REG_COMBO_PLL_TOP_20[7:4]: APLL clock source select
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_ConfigFadingEvent()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_ConfigFadingEvent(Bool bEnable, BYTE ucEvents)
{
    msWriteByte(REG_16308E, bEnable? ucEvents: 0); // REG_COMBO_GP_TOP_47[7:0]: Enable HDMI/DP mute events to trigger fading out
    //[0]: HDMI CTS/N over range / DP audio sample error
    //[1]: HDMI TMDS clock big change / DP audio sample parity error
    //[2]: HDMI AVMUTE / DP non-PCM
    //[3]: HDMI no input clock / DP audio mute (VB_ID[4])
    //[5]: HDMI audio channel status change / DP no input recovery clock
    //[7]: HDMI audio sample error / DP audio FIFO error
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_ConfigMuteEvent()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_ConfigMuteEvent(Bool bEnable, BYTE ucEvents)
{
    msWriteByte(REG_16308B, bEnable? ucEvents: 0); // REG_COMBO_GP_TOP_45[7:0]: Audio mute pin mute event enable
    //[0]: HDMI CTS/N over range / DP audio sample error
    //[1]: HDMI TMDS clock big change / DP audio sample parity error
    //[2]: HDMI AVMUTE / DP non-PCM
    //[3]: HDMI no input clock / DP audio mute (VB_ID[4])
    //[5]: HDMI audio channel status change / DP no input recovery clock
    //[7]: HDMI audio sample error / DP audio FIFO error
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_ConfigGlobalMute()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_ConfigGlobalMute(Bool bEnable)
{
    msWriteByteMask(REG_163088, bEnable? BIT4: 0, BIT4); // REG_COMBO_GP_TOP_44[4]: Audio global mute
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_GetDVIDualMainPort()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
BYTE mhal_combo_GetDVIDualMainPort(BYTE ucPortSelect)
{
    if(ucPortSelect %2) // Port 1/3/5
    {
        ucPortSelect = ucPortSelect -1;
    }
    else // Port 0/2/4
    {
        ucPortSelect = ucPortSelect +1;
    }

    return ucPortSelect;
}

#if(COMBO_HDCP2_FUNCTION_SUPPORT)
//**************************************************************************
//  [Function Name]:
//                  mhal_combo_HDCP2Initial()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_combo_HDCP2Initial(void)
{
    BYTE i = 0;

	msWriteByteMask(0x163062, 0x00, 0xFF); //enable clk
	msWriteByteMask(0x163080, 0x05, 0x07); //enable xiu
	
    #if (COMBO_HDCP2_HDMI20PORT_EN == 1)
    	//tmp: wilson test; write 74ram for HDCP22 version
    	msWriteByteMask(REG_16102F, BIT2, BIT2); //HDCP enable for DDC
    	msWriteByteMask(REG_161033, BIT7, MASKBIT(7:6)); //write enable


    	msWrite2ByteMask(REG_16102E, 0x50, MASKBIT(9:0)); //CPU r/w address (for hdcp_key_sram/74reg)
    	msWriteByteMask(REG_161032, BIT5, BIT5); //HDCP address load pulse generate 				  
    	msWriteByte(REG_161030, 0x04); //bit2: HDCP22 version				   
    	msWriteByteMask(REG_161032, BIT4, BIT4); // REG_COMBO_DVI_HDCP_DUAL_19[4]: HDCP data write port pulse generate 

    	//clear RxStatus wirte msg length
    	RIU[REG_1610C6] = 0x00;
    	msWriteByteMask(REG_1610C9, BIT7, BIT7);
    #endif

    #if (COMBO_HDCP2_HDMI14PORT_EN == 1)
        //for 1.4 port
        for ( i = 0; i < 2; i++ )
        {
            //write 74ram for HDCP22 version
        	msWriteByteMask(REG_16042F + i*0x500, BIT2, BIT2); //HDCP enable for DDC
        	msWriteByteMask(REG_160433 + i*0x500, BIT7, MASKBIT(7:6)); //write enable

        	msWrite2ByteMask(REG_16042E + i*0x500, 0x50, MASKBIT(9:0)); //CPU r/w address (for hdcp_key_sram/74reg)
        	msWriteByteMask(REG_160432 + i*0x500, BIT5, BIT5); //HDCP address load pulse generate 				  
        	msWriteByte(REG_160430 + i*0x500, 0x04); //bit2: HDCP22 version
        	msWriteByteMask(REG_160432 + i*0x500, BIT4, BIT4); // REG_COMBO_DVI_HDCP_DUAL_19[4]: HDCP data write port pulse generate 

        	//clear RxStatus wirte msg length
        	RIU[REG_1604C6 + i*0x500] = 0x00;
            msWriteByteMask(REG_1604C9 + i*0x500, BIT7, BIT7);
        }
    #endif

	msWriteByte(REG_11381E, 0x02); // REG_DPCD_HDCP22_2_0F, reg_rx_caps[0:7]
	msWriteByte(REG_11381F, 0x00); // REG_DPCD_HDCP22_2_0F, reg_rx_caps[15:8]
	msWriteByte(REG_113820, 0x02); // REG_DPCD_HDCP22_2_0F, reg_rx_caps[23:16]

}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_HDCP2GetReadDownFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_combo_HDCP2GetReadDownFlag(BYTE ucPortSelect)
{
    Bool bReadDownFlag = FALSE;
    
    switch(ucPortSelect)
    {
        case COMBO_INPUT_SELECT_PORT0:
            bReadDownFlag = ((msReadByte(REG_1604CC) & BIT3) ? TRUE : FALSE); // REG_HDCP_P0_66[3]: Read down flag

            if(bReadDownFlag)
            {
                msWriteByteMask(REG_1604CC, BIT3, BIT3); // REG_HDCP_P0_66[3]: Clear read down flag
            }
            
            break;

        case COMBO_INPUT_SELECT_PORT1:
            bReadDownFlag = ((msReadByte(REG_1609CC) & BIT3) ? TRUE : FALSE); // REG_HDCP_P1_66[3]: Read down flag

            if(bReadDownFlag)
            {
                msWriteByteMask(REG_1609CC, BIT3, BIT3); // REG_HDCP_P1_66[3]: Clear read down flag
            }
            
            break;

        case COMBO_INPUT_SELECT_PORT2:
        case COMBO_INPUT_SELECT_PORT3:
            bReadDownFlag = ((msReadByte(REG_1610CC) & BIT3) ? TRUE : FALSE); // REG_HDCP_DUAL_66[3]: Read down flag

            if(bReadDownFlag)
            {
                msWriteByteMask(REG_1610CC, BIT3, BIT3); // REG_HDCP_DUAL_66[3]: Clear read down flag
            }
            
            break;

        case COMBO_INPUT_SELECT_PORT4:

            bReadDownFlag = ((msReadByte(REG_113ADF) & BIT7) ? TRUE : FALSE); // H' flag

            if(bReadDownFlag)
            {
				msWriteByteMask(REG_113ADF, 0, BIT7); // H' flag
            }

            break;

        default:

            break;
    };

    return bReadDownFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_combo_HDCP2CheckRomCodeResult()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
Bool mhal_combo_HDCP2CheckRomCodeResult(void)
{
#define HDCP22_MBX_START_ADDR 0x103300
    BYTE i = 0;
    const BYTE ucPattern[4] = {'E', 'N', 'O', 'D'};

    for ( i = 0; i < sizeof(ucPattern); i++ )
    {
        if (msReadByte(HDCP22_MBX_START_ADDR + i) != ucPattern[i])
            return FALSE;
    }
    return TRUE;
}

#endif

#endif


