
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "board.h"
#include "board.h"
#include "types.h"
#include "ms_rwreg.h"
#include "ms_reg.h"
#include "misc.h"
#include "msDAC.h"
#if MS_DAC
bit bToggleGainFlag = 0;
BYTE ToggleGainCntr = 0;
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  msAudioDAC_Init()
//  [Description]
//                  Audio DAC Init
//  [Arguments]:
//
//  [Return]:
//
//************************************************************************** 
void msAudioDAC_Init(void)
{
    BYTE ucBank;

    ToggleGainCntr = 0;
    bToggleGainFlag = 0;

    ucBank = mStar_ReadByte(BK0_00); // store bank
    mStar_WriteByte(BK0_00, REGBANKA); // select register bank DAC

#ifdef TSUMXXQX
    mStar_WriteByte(0x02, 0x0f);  // software reset
    mStar_WriteByte(0x03, 0x00); // software reset
    Delay1ms(1);  // delay 10ms

    mStar_WriteByte(0x02, 0x00);  // software reset
    mStar_WriteByte(0x03, 0x00); // software reset
    // Audio clock config
    // [7]: Audio CH1 enable; [6]: Audio Ch1 256FS enable; [5]: Audio CH1 SDM enable; [4]: Audio CH1 MAC enable; [3]: Audio CH1 DAC enable; [2:0]: Audio CH1 SDM clock = 1/36.
    mStar_WriteByte(0x10, 0xF8);
    // [2]: 256 FS IN; [1]: CLK_128_FS_IN; [0]: 216MHz
    mStar_WriteByte(0x11, 0x00);  // clock enable

    // SRAM config
    // [3]: Clear FIFO status bits(Normal); [2]: Clear MAC status bits(Normal); [1]: Filter SRAM chip disable; [0]: Internal DATA SRAM Initialization
    mStar_WriteByte(0x36, 0x01);  // initial SRAM
    Delay1ms(1);
    mStar_WriteByte(0x37, 0x00);  // initial SRAM
    Delay1ms(5);

    mStar_WriteByte(0x36, 0x00);  // initial SRAM
    Delay1ms(1);
    mStar_WriteByte(0x37, 0x00);  // initial SRAM

    mStar_WriteByte(0x34, 0x07);  // SDM setting
    mStar_WriteByte(0x35, 0x00);  // SDM setting


    // ADAC configuration
    // [6]: Audio referenece voltage resistor string enable
    mStar_WriteByte(0x60, 0x40);   // enable DAC
    // [5:4]: AA switch0(Line-in) zero-cross detection mode(L&R); [3:2]: AA switch0 time-out control(L&R)
    mStar_WriteByte(0x61, 0x30);  // enable DAC
    // [7:6]: AA switch0(Line-in) left channel gain[1:0]; [5:0]: AA switch0(Line-in) right channel gain 0x27 = 0dB
    mStar_WriteByte(0x62, 0x00);
    // [7:6]: AA switch0(Line-in) input select(DAC); [3:0]: AA switch0(Line-in) left channel gain[5:2]
    mStar_WriteByte(0x63, 0x80);   // enable DAC
    // Audio bias current select
    mStar_WriteByte(0x64, 0x55);
    // Audio EAR bias current select
    mStar_WriteByte(0x65, 0x01); // enable DAC
    // Audio power mode
    mStar_WriteByte(0x66, 0x00);
    // [3:2]: Earphone input patch select(Line-out)
    mStar_WriteByte(0x67, 0x08);  // enable DAC

    mStar_WriteByte(0x68, 0x00);
    // [3:2]: Earphone input patch select(Line-out)
    mStar_WriteByte(0x69, 0x30);  // enable DAC
    // Audio AA switch power mode
    mStar_WriteByte(0x6A, 0x00); // designer check AA switch power down polarity
    // earphone switch control
//   msAudioEAR_SourceSel(E_EAROUT_FROM_LINEOUT);

    mStar_WriteByte(0x6B, 0x70);
    Delay1ms(5);


    // Toggle LTO RTO
    {
        // ADAC configuration
        // [6]: Audio referenece voltage resistor string enable
        mStar_WriteByte(0x60, 0x40);   // enable DAC
        // [5:4]: AA switch0(Line-in) zero-cross detection mode(L&R); [3:2]: AA switch0 time-out control(L&R)
        mStar_WriteByte(0x61, 0x3c);  // enable DAC

        // [6]: Audio referenece voltage resistor string enable
        mStar_WriteByte(0x60, 0x40);   // enable DAC
        // [5:4]: AA switch0(Line-in) zero-cross detection mode(L&R); [3:2]: AA switch0 time-out control(L&R)
        mStar_WriteByte(0x61, 0x30);  // enable DAC
    }

    // DWA config
    // [7]: DWA shift disable; [6:5]: SDM dither disable; [2]: DWA Mode2 enable; [1]: DWA output fix disable; [0]: Dither Mode2 disable
    mStar_WriteByte(0x32, 0x04);  // DWA dither setting
    mStar_WriteByte(0x33, 0x00);  // DWA dither setting
    // SDM DC offset
    //mStar_WriteByte(0x3C, 0x80);
    mStar_WriteByte(0x3c, 0x80);  // DC offset
    mStar_WriteByte(0x3d, 0x00);  // DC offset


//    mStar_WriteByte(0x04, 0x10);  // enable PCM gen(1KHz)
//    mStar_WriteByte(0x05, 0x02);   // enable PCM gen(1KHz)

    //[2]: fading gain of DPGA; [0]: DPGA enable
    mStar_WriteByte(0x20, 0x07);  // enable DPGA_1 (fading enable)
//    mStar_WriteByte(0x20, 0x03);  // enable DPGA_1 (fading disable for simulate only)
    msAudioLineout_Mute();
//    mStar_WriteByte(0x61, 0x30); // toggle LTO and RTO
//    mStar_WriteByte(0x61, 0x3C); // toggle LTO and RTO
   mStar_WriteByte(0x21, 0x00);  // enable DPGA_1

   mStar_WriteByte(0x2A ,0x00);  //   DPGA_1 volume
   mStar_WriteByte(0x2B, 0x00);  //   DPGA_1 voluem
    // Step size of fading in L-CH and R-CH, 16 samples.
//    mStar_WriteByte(0x22, 0x03);
//    mStar_WriteByte(0x23, 0x03);

   //Enable HDMI DAC colck
    #if 0
    mStar_WriteByte(BK0_00, REG_BANK_HDMI);
    mStar_WriteByte(0x05, 0x07);
    mStar_WriteByte(0x2E, 0x03);
    mStar_WriteByte(0x2F, 0x05);

    mStar_WriteByte(BK0_00, ucBank);
    #endif

#else // !TSUMXXQX
    // Audio reset
    // [2]: audio clock.gen software reset; [1]: Back-end software reset; [0]: Front-end software reset
    mStar_WriteByte(0x02, 0x07);
    mStar_WriteByte(0x02, 0x00);

    // Audio clock config
    // [7]: Audio CH1 enable; [6]: Audio Ch1 256FS enable; [5]: Audio CH1 SDM enable; [4]: Audio CH1 MAC enable; [3]: Audio CH1 DAC enable; [2:0]: Audio CH1 SDM clock = 1/36.
    mStar_WriteByte(0x10, 0xF8);
    // [2]: 256 FS IN; [1]: CLK_128_FS_IN; [0]: 216MHz
    mStar_WriteByte(0x11, 0x02);
    // 091008 reserved for unnecessary
    // [2]: REG_256FS_EXPANDER; [1]: REG_SYN_ENABLE; [0]: REG_SYN_CHANGE
    //mStar_WriteByte(0x12, 0x02);
    
    // SRAM config
    // [3]: Clear FIFO status bits(Normal); [2]: Clear MAC status bits(Normal); [1]: Filter SRAM chip disable; [0]: Internal DATA SRAM Initialization 
    mStar_WriteByte(0x36, 0x01);
    Delay1ms(1);
    mStar_WriteByte(0x36, 0x00);

    // SDM config
    // [2]: SDM 2nd order; [1]: SDM DEMO enable; [0]: SDM Mode2
    mStar_WriteByte(0x34, 0x07);
    // [0]: MULT_BYPASS
    mStar_WriteByte(0x40, 0x01);
    // [7:5]: 3'b 010 MULT_BYPASS      
    mStar_WriteByte(0x44, 0x40);

    // ADAC configuration
    // [6]: Audio referenece voltage resistor string enable
    mStar_WriteByte(0x60, 0x40);

    // make sure -60dB be set, and RTO/LTO stays at low
    // [5:4]: AA switch0(Line-in) zero-cross detection mode(L&R); [3:2]: AA switch0 time-out control(L&R)
    mStar_WriteByte(0x61, 0x3C);
    mStar_WriteByte(0x61, 0x30);

    // 091008 reserved for unnecessary
    // [7:6]: AA switch0(Line-in) left channel gain[1:0]; [5:0]: AA switch0(Line-in) right channel gain 0x27 = 0dB
    //mStar_WriteByte(0x62, 0xE7);
    // [7:6]: AA switch0(Line-in) input select(DAC); [3:0]: AA switch0(Line-in) left channel gain[5:2]
    //mStar_WriteByte(0x63, 0x89);

    // 091008 follow
    // Audio bias current select
    mStar_WriteByte(0x64, 0x6A);
    // Audio EAR bias current select
    mStar_WriteByte(0x65, 0x01);
    // Audio power mode
    mStar_WriteByte(0x66, 0x00);
    // [3:2]: Earphone input patch select(Line-out)
    mStar_WriteByte(0x67, 0x08);
    // Audio AA switch power mode
    mStar_WriteByte(0x6A, 0x00); // designer check AA switch power down polarity

    // 091008 reserved for strange setting
    // coding for using earphone, not ready
    // earphone switch control
   //msAudioEAR_SourceSel(E_EAROUT_FROM_LINEOUT);
    //mStar_WriteByte(0x6B, 0x03);
    //Delay1ms(3);
    
    
    // DWA config
    // [7]: DWA shift disable; [6:5]: SDM dither disable; [2]: DWA Mode2 enable; [1]: DWA output fix disable; [0]: Dither Mode2 disable
    mStar_WriteByte(0x32, 0x04);
    // SDM DC offset
    //mStar_WriteByte(0x3C, 0x80);

    //[2]: fading gain of DPGA; [0]: DPGA enable
    mStar_WriteByte(0x20, 0x05);
    msAudioLineout_Mute();

    // 091008 reserved for unnecessary
    //mStar_WriteByte(0x61, 0x30); // toggle LTO and RTO
    //mStar_WriteByte(0x61, 0x3C); // toggle LTO and RTO

    // Step size of fading in L-CH and R-CH, 16 samples.
    mStar_WriteByte(0x22, 0x03);
    mStar_WriteByte(0x23, 0x03);	
    
    mStar_WriteByte(0x24, 0x00);
    mStar_WriteByte(0x25, 0x00);	
   //Enable HDMI DAC colck
    #if 0
    mStar_WriteByte(BK0_00, REG_BANK_HDMI);
    mStar_WriteByte(0x05, 0x07);
    mStar_WriteByte(0x2E, 0x03);
    mStar_WriteByte(0x2F, 0x05);

    mStar_WriteByte(BK0_00, ucBank);
    #endif

#endif

    mStar_WriteByte(BK0_00, ucBank);
}


//**************************************************************************
//  [Function Name]:
//                  msAudioLineout_Mute()
//  [Description]
//                  Mute Line output
//  [Arguments]:
//
//  [Return]:
//
//************************************************************************** 
void msAudioLineout_Mute(void)
{
    BYTE ucBank;


    ucBank = mStar_ReadByte(BK0_00); // store bank
    mStar_WriteByte(BK0_00, REGBANKA); // select register bank DAC    

    mStar_WriteByte(0x2A, 0x40); // L-CH: -448
    mStar_WriteByte(0x2B, 0x02);
    mStar_WriteByte(0x2C, 0x40); // R-CH: -448
    mStar_WriteByte(0x2D, 0x02);

    mStar_WriteByte(BK0_00, ucBank);
}


//**************************************************************************
//  [Function Name]:
//                  msAudioLineout_SetVolume()
//  [Description]
//                  Set Line output volume
//  [Arguments]:
//                  chsel: LR channel selection
//                  volume: volume control(+48 ~ -112dB)
//  [Return]:
//
//************************************************************************** 
void msAudioLineout_SetVolume(AUDIO_CH_SEL chsel, int volume)
{
    BYTE ucBank;


    ucBank = mStar_ReadByte(BK0_00); // store bank
    mStar_WriteByte(BK0_00, REGBANKA); // select register bank DAC
    
    if (volume > 192) // +48dB
        volume = 192; //maximum

    if (volume < -448) // -112dB
        volume = -448; //minimum
    
    switch(chsel)
    {
        case E_AUDIO_LEFT_CH:
            mStar_WriteByte(0x2A, (volume&0xFF));
            mStar_WriteByte(0x2B, ((volume>>8)&0x03));
            break;
			
        case E_AUDIO_RIGHT_CH :
            mStar_WriteByte(0x2C, (volume&0xFF));
            mStar_WriteByte(0x2D, ((volume>>8)&0x03));
            break;
			
        case E_AUDIO_LEFT_RIGHT_CH:
	 default:
            mStar_WriteByte(0x2A, (volume&0xFF));
            mStar_WriteByte(0x2B, ((volume>>8)&0x03));
            mStar_WriteByte(0x2C, (volume&0xFF));
            mStar_WriteByte(0x2D, ((volume>>8)&0x03));
            break;
    }
	
    mStar_WriteByte(BK0_00, ucBank);
}

//**************************************************************************
//  [Function Name]:
//                  msAudioLineout_SourceSel()
//  [Description]
//                  Select Line-out source
//  [Arguments]:
//                  src: Line-in0, Line-in1, DAC
//  [Return]:
//
//************************************************************************** 
void msAudioLineout_SourceSel(AUDIO_LINEOUT_SOURCE_SEL src)
{
    BYTE ucBank;
    
    ucBank = mStar_ReadByte(BK0_00); // store bank
    mStar_WriteByte(BK0_00, REGBANKA); // select register bank DAC    

    if((mStar_ReadByte(0x63)&0xc0) != (src<<6))
    {
        // 091008 decrease line-out gain to -60dB step by step
        BYTE tempCnt = mStar_ReadByte(0x62) & 0x3f;
        tempCnt = (tempCnt > 0)?(tempCnt-1):(0);
        do{
                mStar_WriteByte(0x62, tempCnt|((tempCnt&0x03)<<6)); 
                mStar_WriteByte(0x63, (mStar_ReadByte(0x63)&0xc0)|(tempCnt>>2)); 
                // it needs a rising edge to load gain immediately
                mStar_WriteByte(0x61, mStar_ReadByte(0x61)|(BIT2|BIT3));
                mStar_WriteByte(0x61, mStar_ReadByte(0x61)&~(BIT2|BIT3));
                Delay1ms(1);
        }while(--tempCnt < 0x3f);
        
        // set line-out source
        mStar_WriteByte(0x63, src<<6);		
    }
    
    mStar_WriteByte(BK0_00, ucBank);
}


//**************************************************************************
//  [Function Name]:
//                  msAudioLineout_GetSourceSel()
//  [Description]
//                  Get Line-out source selection
//  [Arguments]:
//                 
//  [Return]:
//                  Line-out source selection
//************************************************************************** 
/* AUDIO_LINEOUT_SOURCE_SEL msAudioLineout_GetSourceSel(void)
{
    BYTE ucBank, reg_val;
    AUDIO_LINEOUT_SOURCE_SEL ret_val;


    ucBank = mStar_ReadByte(BK0_00); // store bank
    mStar_WriteByte(BK0_00, REGBANKA); // select register bank DAC    

    reg_val = mStar_ReadByte(0x63);
    ret_val = (reg_val & 0xC0) >> 6; 

    mStar_WriteByte(BK0_00, ucBank);
	
    return ret_val;
}*/

//**************************************************************************
//  [Function Name]:
//                  msAudioEARout_Mute()
//  [Description]
//                  Mute control earphone output
//  [Arguments]:
//                  bEanble: mute on/off
//  [Return]:
//
//************************************************************************** 
void msAudioEARout_MuteOnOff(Bool bEnable)
{
    BYTE ucBank, reg_val;


    ucBank = mStar_ReadByte(BK0_00); // store bank
    mStar_WriteByte(BK0_00, REGBANKA); // select register bank DAC    

    reg_val = mStar_ReadByte(0x6B);
	
    if(bEnable) // Mute on
    {
       // reg_val |= BIT2; // Turn off HPDRV blocks
        reg_val &= (~0xC0);
    }
    else // mute 0ff
    {
	     if(!(reg_val & 0xF0))
	    {
	    	reg_val |= 0x30;
		mStar_WriteByte(0x6B, reg_val);
		Delay1ms(3);
		reg_val &= (~0x03);
		mStar_WriteByte(0x6B, reg_val); 
		Delay1ms(3);
		reg_val = mStar_ReadByte(0x6B);
	    }
	reg_val |= 0xC0;
        //reg_val &= (~BIT2);
    }
    mStar_WriteByte(0x6B, reg_val);

    mStar_WriteByte(BK0_00, ucBank);
}

//**************************************************************************
//  [Function Name]:
//                  msAudioEAR_SourceSel()
//  [Description]
//                  Select Earphone-out source
//  [Arguments]:
//                  src: Line-in0, Line-in1, Line-out, DAC
//  [Return]:
//
//************************************************************************** 
/*
void msAudioEAR_SourceSel(AUDIO_EAROUT_SOURCE_SEL src)
{
    BYTE ucBank, reg_val;


    ucBank = mStar_ReadByte(BK0_00); // store bank
    mStar_WriteByte(BK0_00, REGBANKA); // select register bank DAC    

    reg_val = mStar_ReadByte(0x67);
    mStar_WriteByte(0x67, (reg_val & 0xF3)|(src<<2));
	
    mStar_WriteByte(BK0_00, ucBank);
}
*/
//**************************************************************************
//  [Function Name]:
//                  msAudioEARout_GetSourceSel()
//  [Description]
//                  Get Earphone-out source selection
//  [Arguments]:
//                 
//  [Return]:
//                  Earphone-out source selection
//************************************************************************** 
/* AUDIO_EAROUT_SOURCE_SEL msAudioEARout_GetSourceSel(void)
{
    BYTE ucBank, reg_val;
    AUDIO_LINEOUT_SOURCE_SEL ret_val;


    ucBank = mStar_ReadByte(BK0_00); // store bank
    mStar_WriteByte(BK0_00, REGBANKA); // select register bank DAC    

    reg_val = mStar_ReadByte(0x67);
    ret_val = (reg_val & 0x0C) >> 2;

    mStar_WriteByte(BK0_00, ucBank);
	
    return ret_val;
}*/


//**************************************************************************
//  [Function Name]:
//                  msAudioDACPCMGenerator()
//  [Description]
//                  Internal DAC PCM generator
//  [Arguments]:
//                  bEanble: enable or disable
//                  eFreq: audio frequency
//  [Return]:
//
//************************************************************************** 
/*void msAudioDACPCMGenerator(Bool bEnable, AUDIO_PCM_FREQ eFreq)
{
	 BYTE ucBank;

	 ucBank = mStar_ReadByte(BK0_00); // store bank
    	 mStar_WriteByte(BK0_00, REGBANKA); // select register bank DAC
	 
	if(bEnable)
	{
        mStar_WriteByte(0x04, mStar_ReadByte(0x04) | BIT4);
	}
    else 
    {
        mStar_WriteByte(0x04, mStar_ReadByte(0x04) & (~BIT4));
	}

    mStar_WriteByte(0x05, (mStar_ReadByte(0x05) & 0xF0)|eFreq);

	mStar_WriteByte(BK0_00, ucBank);
}*/

//**************************************************************************
//  [Function Name]:
//                  msAudioDACPowerDown()
//  [Description]
//                  Audio DAC Power Down Control
//  [Arguments]:
//                  bPowerDN: power down
//  [Return]:
//
//************************************************************************** 
void msAudioDACPowerDown(Bool bPowerDN)
{
    BYTE ucBank;

    ToggleGainCntr = 0;
    bToggleGainFlag = 0;

    ucBank = mStar_ReadByte(BK0_00); // store bank
    mStar_WriteByte(BK0_00, REGBANKA); 
    if(bPowerDN)
    {
        // set line-out gain to -60dB step by step
        BYTE tempCnt = mStar_ReadByte(0x62) & 0x3f;
        tempCnt = (tempCnt > 0)?(tempCnt-1):(0);
        do{
                mStar_WriteByte(0x62, tempCnt|((tempCnt&0x03)<<6)); 
                mStar_WriteByte(0x63, (mStar_ReadByte(0x63)&0xc0)|(tempCnt>>2)); 
                // it needs a rising edge to load gain immediately
                mStar_WriteByte(0x61, mStar_ReadByte(0x61)|(BIT2|BIT3));
                mStar_WriteByte(0x61, mStar_ReadByte(0x61)&~(BIT2|BIT3));
                Delay1ms(1);
        }while(--tempCnt < 0x3f);
    
        mStar_WriteByte(0x2A, 0x40); // L-CH: -448
        mStar_WriteByte(0x2B, 0x02);
        mStar_WriteByte(0x2C, 0x40); // R-CH: -448
        mStar_WriteByte(0x2D, 0x02);
        
        mStar_WriteByte(0x65, 0x10);
        mStar_WriteByte(0x6A, 0xFF);
        mStar_WriteByte(0x6B, 0x07); // 091008 [bit3]=0, DC offset always keeps at 1.65V just for earphone.
    }
    else
    {
        mStar_WriteByte(0x65, 0x01);
        mStar_WriteByte(0x6A, 0x00);
        mStar_WriteByte(0x6B, 0xF0);
    }
    mStar_WriteByte(BK0_00, ucBank);

}
// 090617 coding, add new function for controlling line out/dac out gain.
void msAudioLineOutGain(BYTE val)
{
    BYTE tempBank = mStar_ReadByte(0x00);
    mStar_WriteByte(BK0_00, REGBANKA); 
    if(val > 0x3f)
        val = 0x3f;
    
    mStar_WriteByte(0x62, val|((val&0x03)<<6)); 
    mStar_WriteByte(0x63, (mStar_ReadByte(0x63)&0xc0)|(val>>2)); 
    ToggleGainCntr = ToggleGainPeriod;
    bToggleGainFlag = 0;
    
    mStar_WriteByte(BK0_00, tempBank);
}

void msAudioGainToggle(void)
{
    if(bToggleGainFlag)
    {
        BYTE tempBank = mStar_ReadByte(0x00);
        mStar_WriteByte(BK0_00, REGBANKA); 
        mStar_WriteByte(0x61, mStar_ReadByte(0x61)|(BIT2|BIT3));
        mStar_WriteByte(0x61, mStar_ReadByte(0x61)&~(BIT2|BIT3));
        mStar_WriteByte(BK0_00, tempBank); 
        bToggleGainFlag = 0;
    }
}
#endif
