/******************************************************************************
 Copyright (c) 2003 MStar Semiconductor, Inc.
 All rights reserved.

 [Module Name]: Isr.h
 [Date]:        05-Feb-2004
 [Comment]:
   Remote control function header file.
 [Reversion History]:
*******************************************************************************/

#include "Board.h"






#ifndef _IRFUNC_H_
#define _IRFUNC_H_

//#include "board.h"
//#include "define.h"
#include "Types.h"
#include "MsIr.h"

#ifdef _IRFUNC_C_
#define _IRFUNCDEC_
#else
#define _IRFUNCDEC_ extern
#endif

#if(IR_REMOTE_SEL != NO_REMOTE)
//////////////////////////////////////////////////////
// Definition
//////////////////////////////////////////////////////
// tuner change channel delay time
#define IR_NUMKEY_DELAY_TIME1 (3000 / ISR_TIMER0_INTERVAL) // IR 0-9 key press ending delay time (1st key)
#define IR_NUMKEY_DELAY_TIME2 (1000 / ISR_TIMER0_INTERVAL) // IR 0-9 key press ending delay time (2,3 key)


#if(IR_REMOTE_SEL == RC5_MAGNAVOX)
typedef enum _IrCommandType
{
    IRKEY_0             = 0x00,
    IRKEY_1             = 0x01,
    IRKEY_2             = 0x02,
    IRKEY_3             = 0x03,
    IRKEY_4             = 0x04,
    IRKEY_5             = 0x05,
    IRKEY_6             = 0x06,
    IRKEY_7             = 0x07,
    IRKEY_8             = 0x08,
    IRKEY_9             = 0x09,

    IRKEY_100           = 0xFF,

    IRKEY_POWER        = 0x0C,
    IRKEY_INPUT        = 0x38,

    IRKEY_OSD_MENU     = 0x52,
    IRKEY_OSD_LEFT     = 0x11,
    IRKEY_OSD_RIGHT    = 0x10,
    IRKEY_OSD_UP       = 0x20,
    IRKEY_OSD_DOWM     = 0x21,

    IRKEY_PIP          = 0x58,
    IRKEY_TTX_REVEAL    = 0xFD,
    IRKEY_TTX_UPDATE    = 0xFC,
    IRKEY_TTX_SIZE      = 0xFB,
    IRKEY_TTX_CANCEL      = 0xFA,
//    IRKEY_TTX_INDEX     = 0x44,
    IRKEY_TTX_MIX       = 0xF9,
    IRKEY_TTX_INDEX     = 0xF8,
    IRKEY_TTX_HOLD      = 0xF7,
    IRKEY_TTX_LIST      = 0xF6,
    IRKEY_TTX_SUBCODE   = 0xF5,

    IRKEY_TTX_RED       = 0xF4,
    IRKEY_TTX_GREEN     = 0xF3,
    IRKEY_TTX_YELLOW    = 0xF2,
    IRKEY_TTX_CYAN      = 0xF1,

    IRKEY_VOL_UP       = IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM     = IRKEY_TTX_UPDATE,
    IRKEY_CH_UP        = IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM      = IRKEY_TTX_INDEX,

    IRKEY_MUTE         = 0x0D,
    IRKEY_MTS          = 0x23,
    IRKEY_DISPLAY      = 0x49,
    IRKEY_SLEEP        = 0x26,
    IRKEY_ZOOM         = 0x7E,
    IRKEY_PICMODE      = 0x0A,
    IRKEY_SOUNDMODE    = 0x0B,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0x15,


    IRKEY_DUMY         = 0xff
}IrCommandType;
#elif(IR_REMOTE_SEL == RC6_PHILIPS)
typedef enum _IrCommandType
{
    IRKEY_0             = 0x00,
    IRKEY_1             = 0x01,
    IRKEY_2             = 0x02,
    IRKEY_3             = 0x03,
    IRKEY_4             = 0x04,
    IRKEY_5             = 0x05,
    IRKEY_6             = 0x06,
    IRKEY_7             = 0x07,
    IRKEY_8             = 0x08,
    IRKEY_9             = 0x09,

    IRKEY_100           = 0xFF,

    IRKEY_POWER        = 0x0C,
    IRKEY_INPUT        = 0x38,

    IRKEY_OSD_MENU     = 0x54,
    IRKEY_OSD_LEFT     = 0x5A,
    IRKEY_OSD_RIGHT    = 0x5B,
    IRKEY_OSD_UP       = 0x58,
    IRKEY_OSD_DOWM     = 0x59,

    IRKEY_PIP          = 0xD9,
    IRKEY_TTX_REVEAL    = 0xFD,
    IRKEY_TTX_UPDATE    = 0xFC,
    IRKEY_TTX_SIZE      = 0xFB,
    IRKEY_TTX_CANCEL      = 0xFA,
//    IRKEY_TTX_INDEX     = 0x44,
    IRKEY_TTX_MIX       = 0xF9,
    IRKEY_TTX_INDEX     = 0xF8,
    IRKEY_TTX_HOLD      = 0xF7,
    IRKEY_TTX_LIST      = 0xF6,
    IRKEY_TTX_SUBCODE   = 0xF5,

    IRKEY_TTX_RED       = 0xEA,
    IRKEY_TTX_GREEN     = 0xEB,
    IRKEY_TTX_YELLOW    = 0xF2,
    IRKEY_TTX_CYAN      = 0xF1,

    IRKEY_VOL_UP       = 0x10,
    IRKEY_VOL_DOWM     = 0x11,
    IRKEY_CH_UP        = 0x20,
    IRKEY_CH_DOWM      = 0x21,

    IRKEY_MUTE         = 0x0D,
    IRKEY_MTS          = 0x4E,
    IRKEY_DISPLAY      = 0xD2,
    IRKEY_SLEEP        = 0x47,
    IRKEY_ZOOM         = 0x3E,
    IRKEY_PICMODE      = 0xF3,
    IRKEY_SOUNDMODE    = 0xF4,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0x15,


    IRKEY_DUMY         = 0xff
}IrCommandType;





#elif(IR_REMOTE_SEL==TW_REMOTE)

typedef enum _IrCommandType
{
    IRKEY_0             = 0x12,
    IRKEY_1             = 0x05,
    IRKEY_2             = 0x06,
    IRKEY_3             = 0x07,
    IRKEY_4             = 0x09,
    IRKEY_5             = 0x0A,
    IRKEY_6             = 0x0B,
    IRKEY_7             = 0x00,
    IRKEY_8             = 0x0E,
    IRKEY_9             = 0x0F,

    IRKEY_100           = 0x1C,

    IRKEY_POWER        = 0x01,
    IRKEY_INPUT        = 0x14,

    IRKEY_OSD_MENU     = 0x1A,
    IRKEY_OSD_LEFT     = 0x16,
    IRKEY_OSD_RIGHT    = 0x17,
    IRKEY_OSD_UP       = 0x0C,
    IRKEY_OSD_DOWM     = 0x10,

    IRKEY_PIP          = 0x08,
    IRKEY_TTX_REVEAL    = 0x1B,
    IRKEY_TTX_UPDATE    = 0x41,
    IRKEY_TTX_SIZE      = 0x42,
    IRKEY_TTX_CANCEL      = 0x43,
//    IRKEY_TTX_INDEX     = 0x44,
    IRKEY_TTX_MIX       = 0x45,
    IRKEY_TTX_INDEX     = 0x47,
    IRKEY_TTX_HOLD      = 0x48,
    IRKEY_TTX_LIST      = 0x49,
    IRKEY_TTX_SUBCODE   = 0x08,

    IRKEY_TTX_RED       = 0x4A,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0x4C,
    IRKEY_TTX_CYAN      = 0x4D,

    IRKEY_VOL_UP       = IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM     = IRKEY_TTX_UPDATE,
    IRKEY_CH_UP        = IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM      = IRKEY_TTX_INDEX,

    IRKEY_MUTE         = 0x04,
    IRKEY_MTS          = 0x1E,
    IRKEY_DISPLAY      = 0x49,
    IRKEY_SLEEP        = 0x13,
    IRKEY_ZOOM         = IRKEY_TTX_RED,
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0x15,



    IRKEY_DUMY         = 0xff
}IrCommandType;
#elif(IR_REMOTE_SEL==MSTAR_REMOTE)

typedef enum _IrCommandType
{
    IRKEY_0             = 0x50,
    IRKEY_1             = 0x49,
    IRKEY_2             = 0x55,
    IRKEY_3             = 0x59,
    IRKEY_4             = 0x4D,
    IRKEY_5             = 0x51,
    IRKEY_6             = 0x5D,
    IRKEY_7             = 0x48,
    IRKEY_8             = 0x54,
    IRKEY_9             = 0x58,

    IRKEY_100           = 0xff, //no this key

    IRKEY_POWER        = 0x46,  //
    IRKEY_INPUT        = 0x04,  //source

    IRKEY_OSD_MENU     = 0x07,
    IRKEY_OSD_LEFT     = 0x06,
    IRKEY_OSD_RIGHT    = 0x1A,
    IRKEY_OSD_UP       = 0x52,
    IRKEY_OSD_DOWM     = 0x13,

    IRKEY_PIP          = 0x40,
    IRKEY_TTX_REVEAL    = 0x4F,
    IRKEY_TTX_UPDATE    = 0x0C,
    IRKEY_TTX_SIZE      = 0x43,
    IRKEY_TTX_CANCEL      = 0x1B,   //Exit

    IRKEY_TTX_MIX       = 0x1C,
    IRKEY_TTX_INDEX     = 0x18,
    IRKEY_TTX_HOLD      = 0x00,
    IRKEY_TTX_LIST      = 0x10,
    IRKEY_TTX_SUBCODE   = 0x53,

    IRKEY_TTX_RED       = 0x47,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0x57,
    IRKEY_TTX_CYAN      = 0x5B,

    IRKEY_VOL_UP       = 0x16,//IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM     = 0x15,//IRKEY_TTX_UPDATE,
    IRKEY_CH_UP        = 0x1F,//IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM      = 0x19,//IRKEY_TTX_INDEX,

    IRKEY_MUTE         = 0x5A,
    IRKEY_MTS          = 0x41,
    IRKEY_DISPLAY      = 0x14,  //Info
    IRKEY_SLEEP        = 0x45,
    IRKEY_ZOOM         = 0x4C,  //Scale    //IRKEY_TTX_RED,
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0xFE,//no this key


    IRKEY_CECLink      = 0x5F,      //Clock Key
    IRKEY_PLAY         = 0x01,
    IRKEY_STOP         = 0x11,
    IRKEY_PAUSE        = 0x1D,

    IRKEY_DUMY         = 0xff
}IrCommandType;


#elif(IR_REMOTE_SEL==MITSUBISHI_REMOTE)

typedef enum _IrCommandType
{
    IRKEY_0             = 0x12,
    IRKEY_1             = 0x05,
    IRKEY_2             = 0x06,
    IRKEY_3             = 0x07,
    IRKEY_4             = 0x09,
    IRKEY_5             = 0x0A,
    IRKEY_6             = 0x0B,
    IRKEY_7             = 0x00,
    IRKEY_8             = 0x0E,
    IRKEY_9             = 0x0F,

    IRKEY_100           = 0x1C,

    IRKEY_POWER        = 0x01,
    IRKEY_INPUT        = 0x14,

    IRKEY_OSD_MENU     = 0x1A,
    IRKEY_OSD_LEFT     = 0x16,
    IRKEY_OSD_RIGHT    = 0x17,
    IRKEY_OSD_UP       = 0x0C,
    IRKEY_OSD_DOWM     = 0x10,

    IRKEY_PIP          = 0x08,
    IRKEY_TTX_REVEAL    = 0x1B,
    IRKEY_TTX_UPDATE    = 0x41,
    IRKEY_TTX_SIZE      = 0x42,
    IRKEY_TTX_CANCEL      = 0x43,
//    IRKEY_TTX_INDEX     = 0x44,
    IRKEY_TTX_MIX       = 0x45,
    IRKEY_TTX_INDEX     = 0x47,
    IRKEY_TTX_HOLD      = 0x48,
    IRKEY_TTX_LIST      = 0x49,
    IRKEY_TTX_SUBCODE   = 0x08,

    IRKEY_TTX_RED       = 0x4A,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0x4C,
    IRKEY_TTX_CYAN      = 0x4D,

    IRKEY_VOL_UP       = IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM     = IRKEY_TTX_UPDATE,
    IRKEY_CH_UP        = IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM      = IRKEY_TTX_INDEX,

    IRKEY_MUTE         = 0x04,
    IRKEY_MTS          = 0x1E,
    IRKEY_DISPLAY      = 0x49,
    IRKEY_SLEEP        = 0x13,
    IRKEY_ZOOM         = IRKEY_TTX_RED,
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,


    IRKEY_DUMY         = 0xff
}IrCommandType;
#elif(IR_REMOTE_SEL==JRY_IR_MODE)

#if (defined(LeShiYouXian_Project))
typedef enum _IrCommandType
{
    IRKEY_0             = 0x51,
    IRKEY_1             = 0x1C,
    IRKEY_2             = 0x1E,
    IRKEY_3             = 0x4C,
    IRKEY_4             = 0x1B,
    IRKEY_5             = 0xFE,//4C,
    IRKEY_6             = 0x4D,
    IRKEY_7             = 0x53,
    IRKEY_8             = 0x54,
    IRKEY_9             = 0x55,

    IRKEY_100           = 0xFF, //no this key

    IRKEY_POWER        = 0x19,
    IRKEY_INPUT        = 0x41,  //source

    IRKEY_OSD_MENU     = 0x0D,
    IRKEY_OSD_LEFT     = 0x05,
    IRKEY_OSD_RIGHT    = 0x4B,
    IRKEY_OSD_UP       = 0x04,
    IRKEY_OSD_DOWM     = 0x44,
    IRKEY_OSD_EXIT     = 0x09,

    IRKEY_PIP          = 0x40,
    IRKEY_TTX_REVEAL    = 0x4F,
    IRKEY_TTX_UPDATE    = 0x0C,
    IRKEY_TTX_SIZE      = 0x43,
    IRKEY_TTX_CANCEL      = 0x1B,   //Exit

    IRKEY_TTX_MIX       = 0x1C,
    IRKEY_TTX_INDEX     = 0x18,
    IRKEY_TTX_HOLD      = 0x00,
    IRKEY_TTX_LIST      = 0x10,
    IRKEY_TTX_SUBCODE   = 0x53,

    IRKEY_TTX_RED       = 0x47,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0x57,
    IRKEY_TTX_CYAN      = 0x5B,

    IRKEY_VOL_UP       = 0x5A,//IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM     = 0x46,//IRKEY_TTX_UPDATE,
    IRKEY_CH_UP        = 0x00,//IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM      = 0x42,//IRKEY_TTX_INDEX,

    IRKEY_MUTE         = 0x5A,
    IRKEY_MTS          = 0x41,
    IRKEY_DISPLAY      = 0x14,  //Info
    IRKEY_SLEEP        = 0x45,
    IRKEY_ZOOM         = 0x4C,  //
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0xFE,//no this key


    IRKEY_CECLink      = 0x5F,      //Clock Key
    IRKEY_PLAY         = 0x01,
    IRKEY_STOP         = 0x11,
    IRKEY_PAUSE        = 0x1D,

    IRKEY_DUMY         = 0xff
}IrCommandType;
#elif (defined(Korean_Project))
typedef enum _IrCommandType
{
    IRKEY_0             = 0x51,
    IRKEY_1             = 0x1C,
    IRKEY_2             = 0x1E,
    IRKEY_3             = 0x4C,
    IRKEY_4             = 0x1B,
    IRKEY_5             = 0xFE,//4C,
    IRKEY_6             = 0x4D,
    IRKEY_7             = 0x53,
    IRKEY_8             = 0x54,
    IRKEY_9             = 0x55,

    IRKEY_100           = 0xFF, //no this key

    IRKEY_POWER        = 0x0C,
    IRKEY_INPUT        = 0x0D,  //source

    IRKEY_OSD_MENU     = 0x0F,
    IRKEY_OSD_LEFT     = 0x16,
    IRKEY_OSD_RIGHT    = 0x15,
    IRKEY_OSD_UP       = 0x17,
    IRKEY_OSD_DOWM     = 0x18,
    IRKEY_OSD_EXIT     = 0x14,

    IRKEY_PIP          = 0x40,
    IRKEY_TTX_REVEAL    = 0x4F,
    IRKEY_TTX_UPDATE    = 0x0C,
    IRKEY_TTX_SIZE      = 0x43,
    IRKEY_TTX_CANCEL      = 0x1B,   //Exit

    IRKEY_TTX_MIX       = 0x1C,
    IRKEY_TTX_INDEX     = 0x18,
    IRKEY_TTX_HOLD      = 0x00,
    IRKEY_TTX_LIST      = 0x10,
    IRKEY_TTX_SUBCODE   = 0x53,

    IRKEY_TTX_RED       = 0x47,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0x57,
    IRKEY_TTX_CYAN      = 0x5B,

    IRKEY_VOL_UP       = 0x16,//IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM     = 0x15,//IRKEY_TTX_UPDATE,
    IRKEY_CH_UP        = 0x1F,//IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM      = 0x19,//IRKEY_TTX_INDEX,

    IRKEY_MUTE         = 0x5A,
    IRKEY_MTS          = 0x41,
    IRKEY_DISPLAY      = 0x14,  //Info
    IRKEY_SLEEP        = 0x45,
    IRKEY_ZOOM         = 0x4C,  //
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0xFE,//no this key


    IRKEY_CECLink      = 0x5F,      //Clock Key
    IRKEY_PLAY         = 0x01,
    IRKEY_STOP         = 0x11,
    IRKEY_PAUSE        = 0x1D,

    IRKEY_DUMY         = 0xff
}IrCommandType;

#elif (defined(LeXingEnMa_Project)||LHCX_IR_Function_Enable)
typedef enum _IrCommandType
{
    IRKEY_0             = 0x51,
    IRKEY_1             = 0xaa,
    IRKEY_2             = 0x1E,
    IRKEY_3             = 0xFA,
    IRKEY_4             = 0x88,
    IRKEY_5             = 0xFE,//4C,
    IRKEY_6             = 0x4D,
    IRKEY_7             = 0x53,
    IRKEY_8             = 0x54,
    IRKEY_9             = 0x55,
    IRKEY_100           = 0xFF, //no this key
    
    IRKEY_POWER        = 0x5C,
    IRKEY_INPUT        = 0x1E,  //source
    
    IRKEY_OSD_MENU     = 0x4C,
    IRKEY_OSD_LEFT     = 0x07,
    IRKEY_OSD_RIGHT    = 0x0F,
    IRKEY_OSD_UP       = 0x48,
    IRKEY_OSD_DOWM     = 0x0A,
    IRKEY_OSD_EXIT     = 0x44,

    IRKEY_OSD_Enter  = 0x0B,
    
    IRKEY_PIP          = 0x11,//0x09,
    IRKEY_PIP_Position          = 0x0E,   //
    IRKEY_PIP_Swap          = 0x4E,
    IRKEY_PIP_AudioSwap          = 0x4A,

	
    IRKEY_TTX_REVEAL    = 0x4F,
    IRKEY_TTX_UPDATE    = 0x0C,
    IRKEY_TTX_SIZE      = 0xFD,
    IRKEY_TTX_CANCEL      = 0x1B,   //Exit
    IRKEY_TTX_MIX       = 0x1C,
    IRKEY_TTX_INDEX     = 0x18,
    IRKEY_TTX_HOLD      = 0x00,
    IRKEY_TTX_LIST      = 0xFC,
    IRKEY_TTX_SUBCODE   = 0x53,
    IRKEY_TTX_RED       = 0x47,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0x57,
    IRKEY_TTX_CYAN      = 0x5B,
    
    IRKEY_VOL_UP       = 0x10,//0x58,//IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM     = 0x1F,//0x5D,//IRKEY_TTX_UPDATE,
    IRKEY_CH_UP        = 0x89,//IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM      = 0x19,//IRKEY_TTX_INDEX,
    
    IRKEY_MUTE         = 0x14,//0x05,
    IRKEY_MTS          = 0x41,
    IRKEY_DISPLAY      = 0xFB,  //Info
    IRKEY_SLEEP        = 0x45,
    IRKEY_ZOOM         = 0x46,//0x4C,  //
    
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0xFE,//no this key
    IRKEY_CECLink      = 0x5F,      //Clock Key
    IRKEY_PLAY         = 0x01,
    IRKEY_STOP         = 0x11,
    IRKEY_PAUSE        = 0x1D,
    IRKEY_DUMY         = 0xff
    
}IrCommandType;


#ifdef IR_MODE_SEL_2
typedef enum _IrCommandType2
{
    
    IRKEY_POWER_2        = 0x08,
    IRKEY_INPUT_2        = 0x0B, 
    
    IRKEY_OSD_MENU_2     = 0x43,
    IRKEY_OSD_LEFT_2     = 0x07,
    IRKEY_OSD_RIGHT_2    = 0x06,
    IRKEY_OSD_UP_2       = 0x40,
    IRKEY_OSD_DOWM_2     = 0x41,
    IRKEY_OSD_EXIT_2     = 0x5B,

    IRKEY_OSD_Enter_2  = 0x44,
    
    IRKEY_PIP_2          = 0xAB,//0x09,
    IRKEY_PIP_Position_2          = 0x39,   //
    IRKEY_PIP_Swap_2          = 0x30,
    IRKEY_PIP_AudioSwap_2          = 0x52,

    
    IRKEY_VOL_UP_2       = 0x02,//0x58,//IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM_2     = 0x03,//0x5D,//IRKEY_TTX_UPDATE,
    IRKEY_CH_UP_2        = 0x89,//IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM_2      = 0x19,//IRKEY_TTX_INDEX,
    
    IRKEY_MUTE_2         = 0x09,//0x05,
    IRKEY_SLEEP_2        = 0x0E,
    IRKEY_ZOOM_2         = 0x79,//0x4C,  //
    

}IrCommandType2;

#endif

#ifdef IR_MODE_SEL_3
typedef enum _IrCommandType3
{
    
    IRKEY_POWER_3        = 0x0A,
    IRKEY_INPUT_3        = 0x1B, 
    
    IRKEY_OSD_MENU_3     = 0x1A,
    IRKEY_OSD_LEFT_3     = 0x03,
    IRKEY_OSD_RIGHT_3    = 0x02,
    IRKEY_OSD_UP_3       = 0x00,
    IRKEY_OSD_DOWM_3     = 0x01,
    IRKEY_OSD_EXIT_3     = 0x1C,

    IRKEY_OSD_Enter_3  = 0x1F,
    
    IRKEY_PIP_3          = 0x05,//0x09,
    IRKEY_PIP_Position_3          = 0x09,   //
    IRKEY_PIP_Swap_3          = 0x42,
    IRKEY_PIP_AudioSwap_3          = 0x43,

   /* 
    IRKEY_VOL_UP_3       = 0x02,//0x58,//IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM_3     = 0x03,//0x5D,//IRKEY_TTX_UPDATE,
    IRKEY_CH_UP_3        = 0x89,//IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM_3      = 0x19,//IRKEY_TTX_INDEX,
  */  
    IRKEY_MUTE_3         = 0x5D,//0x05,
    IRKEY_SLEEP_3        = 0x0E,
    IRKEY_ZOOM_3         = 0x58,//0x4C,  //
    

}IrCommandType3;

#endif
#ifdef IR_MODE_SEL_4
typedef enum _IrCommandType4
{
    
    IRKEY_POWER_4        = 0x04,
    IRKEY_INPUT_4        = 0x00, 
    
    IRKEY_OSD_MENU_4     = 0x16,
    IRKEY_OSD_LEFT_4     = 0x1A,
    IRKEY_OSD_RIGHT_4    = 0x59,
    IRKEY_OSD_UP_4       = 0x17,
    IRKEY_OSD_DOWM_4     = 0x1B,
 //   IRKEY_OSD_EXIT_4     = 0x1C,

    IRKEY_OSD_Enter_4  = 0x42,
    
   /* 
    IRKEY_PIP_4          = 0x05,//0x09,
    IRKEY_PIP_Position_4          = 0x09,   //
    IRKEY_PIP_Swap_4          = 0x42,
    IRKEY_PIP_AudioSwap_4          = 0x43,

    IRKEY_VOL_UP_3       = 0x02,//0x58,//IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM_3     = 0x03,//0x5D,//IRKEY_TTX_UPDATE,
    IRKEY_CH_UP_3        = 0x89,//IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM_3      = 0x19,//IRKEY_TTX_INDEX,
  */  
    IRKEY_MUTE_4         = 0x0F,
    //IRKEY_SLEEP_4        = 0x0E,
    IRKEY_ZOOM_4         = 0x43,
    

}IrCommandType4;

#endif

#ifdef IR_MODE_SEL_5
typedef enum _IrCommandType5
{
    
    IRKEY_POWER_5        = 0x46,
    IRKEY_INPUT_5        = 0x58, 
    
    IRKEY_OSD_MENU_5     = 0x0E,
    IRKEY_OSD_LEFT_5     = 0x4E,
    IRKEY_OSD_RIGHT_5    = 0x49,
    IRKEY_OSD_UP_5      = 0x48,
    IRKEY_OSD_DOWM_5     = 0x4D,
    IRKEY_OSD_EXIT_5     = 0x0D,

    IRKEY_OSD_Enter_5  = 0x4A,
    
    //IRKEY_PIP_5          = 0x5E,
    //IRKEY_PIP_Position_5          = 0x09,   
    //IRKEY_PIP_Swap_5          = 0x42,
    //IRKEY_PIP_AudioSwap_5          = 0x43,
    IRKEY_LOW_BLURAY_5     = 0x12,
    IRKEY_PIC_MODE_5          = 0x5E,
    IRKEY_DCR_5          = 0x5D,
 
    IRKEY_MUTE_5         = 0x5C,
    //IRKEY_SLEEP_5        = 0x0E,
    IRKEY_ZOOM_5         = 0x5F,
    
}IrCommandType5;

#endif

#elif (defined(ZhouYang_Project))

typedef enum _IrCommandType
{
    IRKEY_0             = 0x51,
    IRKEY_1             = 0xaa,
    IRKEY_2             = 0xF9,
    IRKEY_3             = 0xFA,
    IRKEY_4             = 0x88,
    IRKEY_5             = 0xFE,//4C,
    IRKEY_6             = 0x4D,
    IRKEY_7             = 0x53,
    IRKEY_8             = 0x54,
    IRKEY_9             = 0x55,
    IRKEY_100           = 0xFF, //no this key
    
    IRKEY_POWER        = 0x02,
    IRKEY_INPUT        = 0x08,  //source
    
    IRKEY_OSD_MENU     = 0x10,
    IRKEY_OSD_LEFT     = 0x0C,
    IRKEY_OSD_RIGHT    = 0x0E,
    IRKEY_OSD_UP       = 0x09,
    IRKEY_OSD_DOWM     = 0x11,
    IRKEY_OSD_EXIT     = 0x12,

    IRKEY_OSD_Enter  = 0x0D,
    
    IRKEY_PIP          = 0x0A,//0x09,
    IRKEY_PIP_Position          = 0x60,   //
    IRKEY_PIP_Swap          = 0x4E,
    IRKEY_PIP_AudioSwap          = 0x4A,

	
    IRKEY_TTX_REVEAL    = 0x4F,
    IRKEY_TTX_UPDATE    = 0x61,
    IRKEY_TTX_SIZE      = 0xFD,
    IRKEY_TTX_CANCEL      = 0x1B,   //Exit
    IRKEY_TTX_MIX       = 0x1C,
    IRKEY_TTX_INDEX     = 0x18,
    IRKEY_TTX_HOLD      = 0x00,
    IRKEY_TTX_LIST      = 0xFC,
    IRKEY_TTX_SUBCODE   = 0x53,
    IRKEY_TTX_RED       = 0x47,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0x57,
    IRKEY_TTX_CYAN      = 0x5B,
    
    IRKEY_VOL_UP       = 0x62,//0x58,//IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM     = 0x1F,//0x5D,//IRKEY_TTX_UPDATE,
    IRKEY_CH_UP        = 0x89,//IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM      = 0x19,//IRKEY_TTX_INDEX,
    
    IRKEY_MUTE         = 0x14,//0x05,
    IRKEY_MTS          = 0x41,
    IRKEY_DISPLAY      = 0xFB,  //Info
    IRKEY_SLEEP        = 0x42,
    IRKEY_ZOOM         = 0x46,//0x4C,  //
    
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0xFE,//no this key
    IRKEY_CECLink      = 0x5F,      //Clock Key
    IRKEY_PLAY         = 0x01,
    IRKEY_STOP         = 0x63,
    IRKEY_PAUSE        = 0x1D,
    IRKEY_DUMY         = 0xff
    
}IrCommandType;

#elif (JieBo_IR_Function_Enable)

typedef enum _IrCommandType
{
    IRKEY_0             = 0x51,
    IRKEY_1             = 0xaa,
    IRKEY_2             = 0xF9,
    IRKEY_3             = 0xFA,
    IRKEY_4             = 0x88,
    IRKEY_5             = 0xFE,//4C,
    IRKEY_6             = 0x4D,
    IRKEY_7             = 0x53,
    IRKEY_8             = 0x54,
    IRKEY_9             = 0x55,
    IRKEY_100           = 0xFF, //no this key
    
    IRKEY_POWER        = 0x0F,
    IRKEY_INPUT        = 0x12,  //source
    
    IRKEY_OSD_MENU     = 0xDC,
    IRKEY_OSD_LEFT     = 0x97,
    IRKEY_OSD_RIGHT    = 0x9F,
    IRKEY_OSD_UP       = 0x92,
    IRKEY_OSD_DOWM     = 0xD8,
    IRKEY_OSD_EXIT     = 0xD4,

    IRKEY_OSD_Enter  = 0x9B,
    
    IRKEY_PIP          = 0xCF,
    IRKEY_PIP_Size          = 0xDB,   
    IRKEY_PIP_Position          = 0xC1,   
    IRKEY_PIP_Swap          = 0xCC,
    IRKEY_PIP_AudioSwap          = 0x4A,
    
    IRKEY_PBP_OFF          = 0xC0,
    IRKEY_PBP_2WIN          = 0x09,   
    IRKEY_PBP_3WIN          = 0xC7,   
    IRKEY_PBP_4WIN          = 0xC2,

    IRKEY_Source_DP          = 0xD3,
    IRKEY_Source_HDMI1          = 0xCE,   
    IRKEY_Source_HDMI2          = 0xD2,   
    IRKEY_Source_HDMI3          = 0xCD,
	
    IRKEY_TTX_REVEAL    = 0x4F,
    IRKEY_TTX_UPDATE    = 0x61,
    IRKEY_TTX_SIZE      = 0xFD,
    IRKEY_TTX_CANCEL      = 0x1B,   //Exit
    IRKEY_TTX_MIX       = 0x1C,
    IRKEY_TTX_INDEX     = 0x18,
    IRKEY_TTX_HOLD      = 0x00,
    IRKEY_TTX_LIST      = 0xFC,
    IRKEY_TTX_SUBCODE   = 0x53,
    IRKEY_TTX_RED       = 0x47,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0x57,
    IRKEY_TTX_CYAN      = 0x5B,
    
    IRKEY_VOL_UP       = 0x0C,
    IRKEY_VOL_DOWM     = 0x0D,
    IRKEY_CH_UP        = 0x0A,
    IRKEY_CH_DOWM      = 0x0B,
    
    IRKEY_MUTE         = 0x0E,//0x05,
    IRKEY_MTS          = 0x41,
    IRKEY_DISPLAY      = 0xFB,  //Info
    IRKEY_SLEEP        = 0x42,
    IRKEY_ZOOM         = 0x46,//0x4C,  //
    
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0xFE,//no this key
    IRKEY_CECLink      = 0x5F,      //Clock Key
    IRKEY_PLAY         = 0x01,
    IRKEY_STOP         = 0x63,
    IRKEY_PAUSE        = 0x1D,
    IRKEY_DUMY         = 0xff
    
}IrCommandType;


#elif (defined(TaiBei_Project))
typedef enum _IrCommandType
{
    IRKEY_0             = 0x51,
    IRKEY_1             = 0xaa,
    IRKEY_2             = 0xF9,
    IRKEY_3             = 0xFA,
    IRKEY_4             = 0x88,
    IRKEY_5             = 0xFE,//4C,
    IRKEY_6             = 0x4D,
    IRKEY_7             = 0x53,
    IRKEY_8             = 0x54,
    IRKEY_9             = 0x55,
    IRKEY_100           = 0xFF, //no this key
    
    IRKEY_POWER        = 0xD7,
    IRKEY_INPUT        = 0xDB,  //source
    
    IRKEY_OSD_MENU     = 0xDC,
    IRKEY_OSD_LEFT     = 0x97,
    IRKEY_OSD_RIGHT    = 0x9F,
    IRKEY_OSD_UP       = 0x92,
    IRKEY_OSD_DOWM     = 0xD8,
    IRKEY_OSD_EXIT     = 0xD4,

    IRKEY_OSD_Enter  = 0x8A,
    
    IRKEY_PIP          = 0x11,//0x09,
    IRKEY_PIP_Position          = 0x0E,   //
    IRKEY_PIP_Swap          = 0x4E,
    IRKEY_PIP_AudioSwap          = 0x4A,

	
    IRKEY_TTX_REVEAL    = 0x4F,
    IRKEY_TTX_UPDATE    = 0x0C,
    IRKEY_TTX_SIZE      = 0xFD,
    IRKEY_TTX_CANCEL      = 0x1B,   //Exit
    IRKEY_TTX_MIX       = 0x1C,
    IRKEY_TTX_INDEX     = 0x18,
    IRKEY_TTX_HOLD      = 0x00,
    IRKEY_TTX_LIST      = 0xFC,
    IRKEY_TTX_SUBCODE   = 0x53,
    IRKEY_TTX_RED       = 0x47,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0x57,
    IRKEY_TTX_CYAN      = 0x5B,
    
    IRKEY_VOL_UP       = 0x83,
    IRKEY_VOL_DOWM     = 0x86,
    IRKEY_CH_UP        = 0x80,
    IRKEY_CH_DOWM      = 0x8E,
    
    IRKEY_MUTE         = 0xDF,//0x05,
    IRKEY_MTS          = 0x41,
    IRKEY_DISPLAY      = 0xFB,  //Info
    IRKEY_SLEEP        = 0x42,
    IRKEY_ZOOM         = 0xC9,//0x4C,  //
    
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0xFE,//no this key
    IRKEY_CECLink      = 0x5F,      //Clock Key
    IRKEY_PLAY         = 0x01,
    IRKEY_STOP         = 0x11,
    IRKEY_PAUSE        = 0x1D,
    IRKEY_DUMY         = 0xff
    
}IrCommandType;

#ifdef IR_MODE_SEL_2
typedef enum _IrCommandType2
{
    
    IRKEY_POWER_2        = 0x01,
    IRKEY_INPUT_2        = 0x26, 
    
    IRKEY_OSD_MENU_2     = 0x27,
    IRKEY_OSD_LEFT_2     = 0x05,
    IRKEY_OSD_RIGHT_2    = 0x06,
    IRKEY_OSD_UP_2       = 0x03,
    IRKEY_OSD_DOWM_2     = 0x08,
    IRKEY_OSD_EXIT_2     = 0x25,

    IRKEY_OSD_Enter_2  = 0x07,
    
    IRKEY_PIP_2          = 0xAB,//0x09,
    IRKEY_PIP_Position_2          = 0x39,   //
    IRKEY_PIP_Swap_2          = 0x30,
    IRKEY_PIP_AudioSwap_2          = 0x52,

    
    IRKEY_VOL_UP_2       = 0x28,//0x58,//IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM_2     = 0x29,//0x5D,//IRKEY_TTX_UPDATE,
    IRKEY_CH_UP_2        = 0x89,//IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM_2      = 0x19,//IRKEY_TTX_INDEX,
    
    IRKEY_MUTE_2         = 0x01,//0x05,
    IRKEY_SLEEP_2        = 0x0E,
    IRKEY_ZOOM_2         = 0x79,//0x4C,  //
    

}IrCommandType2;

#endif

#ifdef IR_MODE_SEL_3
typedef enum _IrCommandType3
{
    
    IRKEY_POWER_3        = 0x00,
    IRKEY_INPUT_3        = 0x10, 
    
    IRKEY_OSD_MENU_3     = 0x12,
    IRKEY_OSD_LEFT_3     = 0x1C,
    IRKEY_OSD_RIGHT_3    = 0x48,
    IRKEY_OSD_UP_3       = 0x44,
    IRKEY_OSD_DOWM_3     = 0x1D,
    IRKEY_OSD_EXIT_3     = 0x13,

    IRKEY_OSD_Enter_3  = 0x0D,
    
    IRKEY_PIP_3          = 0xF7,//0x09,
    IRKEY_PIP_Position_3          = 0xF8,   //
    IRKEY_PIP_Swap_3          = 0xFA,
    IRKEY_PIP_AudioSwap_3          = 0xF9,


    IRKEY_VOL_UP_3       = 0x19,//0x58,//IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM_3     = 0x15,//0x5D,//IRKEY_TTX_UPDATE,
    IRKEY_CH_UP_3        = 0xFC,//IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM_3      = 0xFD,//IRKEY_TTX_INDEX,

    IRKEY_MUTE_3         = 0x42,//0x05,
    IRKEY_SLEEP_3        = 0xFB,
    IRKEY_ZOOM_3         = 0x54,//0x4C,  //
    

}IrCommandType3;

#endif


#elif (defined(HuaKe_Project))
typedef enum _IrCommandType
{
    IRKEY_0             = 0x51,
    IRKEY_1             = 0xaa,
    IRKEY_2             = 0xF9,
    IRKEY_3             = 0xFA,
    IRKEY_4             = 0x88,
    IRKEY_5             = 0xFE,//4C,
    IRKEY_6             = 0x4D,
    IRKEY_7             = 0x53,
    IRKEY_8             = 0x54,
    IRKEY_9             = 0x55,
    IRKEY_100           = 0xFF, //no this key
    
    IRKEY_POWER        = 0xD7,
    IRKEY_INPUT        = 0xC3,  //source
    
    IRKEY_OSD_MENU     = 0xD4,
    IRKEY_OSD_LEFT     = 0x97,
    IRKEY_OSD_RIGHT    = 0x9F,
    IRKEY_OSD_UP       = 0x92,
    IRKEY_OSD_DOWM     = 0xD8,
    IRKEY_OSD_EXIT     = 0xDC,

    IRKEY_OSD_Enter  = 0x9B,
    
    IRKEY_PIP          = 0x11,//0x09,
    IRKEY_PIP_Position          = 0x0E,   //
    IRKEY_PIP_Swap          = 0x4E,
    IRKEY_PIP_AudioSwap          = 0x4A,

	
    IRKEY_TTX_REVEAL    = 0x4F,
    IRKEY_TTX_UPDATE    = 0x0C,
    IRKEY_TTX_SIZE      = 0xFD,
    IRKEY_TTX_CANCEL      = 0x1B,   //Exit
    IRKEY_TTX_MIX       = 0x1C,
    IRKEY_TTX_INDEX     = 0x18,
    IRKEY_TTX_HOLD      = 0x00,
    IRKEY_TTX_LIST      = 0xFC,
    IRKEY_TTX_SUBCODE   = 0x53,
    IRKEY_TTX_RED       = 0x47,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0x57,
    IRKEY_TTX_CYAN      = 0x5B,
    
    IRKEY_VOL_UP       = 0x83,
    IRKEY_VOL_DOWM     = 0x86,
    IRKEY_CH_UP        = 0x80,
    IRKEY_CH_DOWM      = 0x8E,
    
    IRKEY_MUTE         = 0xDF,//0x05,
    IRKEY_MTS          = 0x41,
    IRKEY_DISPLAY      = 0xFB,  //Info
    IRKEY_SLEEP        = 0x42,
    IRKEY_ZOOM         = 0xDB,//0x4C,  //
    
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0xFE,//no this key
    IRKEY_CECLink      = 0x5F,      //Clock Key
    IRKEY_PLAY         = 0x01,
    IRKEY_STOP         = 0x11,
    IRKEY_PAUSE        = 0x1D,
    IRKEY_DUMY         = 0xff
    
}IrCommandType;
#elif 0//(defined(Japan_Project))
typedef enum _IrCommandType5
{
    IRKEY_0             = 0x51,
    IRKEY_1             = 0xaa,
    IRKEY_2             = 0xF9,
    IRKEY_3             = 0xFA,
    IRKEY_4             = 0x88,
    IRKEY_5             = 0xFE,//4C,
    IRKEY_6             = 0x4D,
    IRKEY_7             = 0xF2,
    IRKEY_8             = 0x54,
    IRKEY_9             = 0x55,
    IRKEY_100           = 0xFF, //no this key
    
    IRKEY_POWER        = 0x57,
    IRKEY_INPUT        = 0x14,
    
    IRKEY_OSD_MENU     =0x19,
    IRKEY_OSD_LEFT     = 0x53,
    IRKEY_OSD_RIGHT    = 0x10,
    IRKEY_OSD_UP       = 0x0D,
    IRKEY_OSD_DOWM     = 0x4F,
    IRKEY_OSD_EXIT     = 0x56,

    IRKEY_OSD_Enter  = 0x11,
    
    IRKEY_PIP          = 0xD1,//0x09,
    IRKEY_PIP_Position          = 0x0E,   //
    IRKEY_PIP_Swap          = 0xDA,
    IRKEY_PIP_AudioSwap          = 0x4A,

	
    IRKEY_TTX_REVEAL    = 0xED,
    IRKEY_TTX_UPDATE    = 0xDB,
    IRKEY_TTX_SIZE      = 0xFD,
    IRKEY_TTX_CANCEL      = 0x1B,   //Exit
    IRKEY_TTX_MIX       = 0x1C,
    IRKEY_TTX_INDEX     = 0x18,
    IRKEY_TTX_HOLD      = 0x00,
    IRKEY_TTX_LIST      = 0xFC,
    IRKEY_TTX_SUBCODE   = 0xF1,
    IRKEY_TTX_RED       = 0x47,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0xF4,
    IRKEY_TTX_CYAN      = 0x5B,
    
    IRKEY_VOL_UP       = 0x4E,
    IRKEY_VOL_DOWM     = 0x0C,
    IRKEY_CH_UP        = 0x80,
    IRKEY_CH_DOWM      = 0x8E,
    
    IRKEY_MUTE         = 0x0F,
    IRKEY_MTS          = 0x41,
    IRKEY_DISPLAY      = 0xFB,  //Info
    IRKEY_SLEEP        = 0x42,
    IRKEY_ZOOM         = 0x43,//0x4C,  //
    
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0xFE,//no this key
    IRKEY_CECLink      = 0x5F,      //Clock Key
    IRKEY_PLAY         = 0x01,
    IRKEY_STOP         = 0xDD,
    IRKEY_PAUSE        = 0x1D,
    IRKEY_DUMY         = 0xff
    
}IrCommandType5;


#elif (defined(FengYuan_Project))
typedef enum _IrCommandType
{
    IRKEY_0             = 0x51,
    IRKEY_1             = 0xaa,
    IRKEY_2             = 0xF9,
    IRKEY_3             = 0xFA,
    IRKEY_4             = 0x88,
    IRKEY_5             = 0xFE,//4C,
    IRKEY_6             = 0x4D,
    IRKEY_7             = 0x53,
    IRKEY_8             = 0x54,
    IRKEY_9             = 0x55,
    IRKEY_100           = 0xFF, //no this key
    
    IRKEY_POWER        = 0x04,
    IRKEY_INPUT        = 0x16,//0x00,  //source
    
    IRKEY_OSD_MENU     =0x00,// 0x16,
    IRKEY_OSD_LEFT     = 0x1A,
    IRKEY_OSD_RIGHT    = 0x59,
    IRKEY_OSD_UP       = 0x17,
    IRKEY_OSD_DOWM     = 0x1B,
    IRKEY_OSD_EXIT     = 0xDB,//0x0F,

    IRKEY_OSD_Enter  = 0x42,
    
    IRKEY_PIP          = 0x0f,//0x09,
    IRKEY_PIP_Position          = 0x0E,   //
    IRKEY_PIP_Swap          = 0x4E,
    IRKEY_PIP_AudioSwap          = 0x4A,

	
    IRKEY_TTX_REVEAL    = 0x4F,
    IRKEY_TTX_UPDATE    = 0x0C,
    IRKEY_TTX_SIZE      = 0xFD,
    IRKEY_TTX_CANCEL      = 0x1B,   //Exit
    IRKEY_TTX_MIX       = 0x1C,
    IRKEY_TTX_INDEX     = 0x18,
    IRKEY_TTX_HOLD      = 0x00,
    IRKEY_TTX_LIST      = 0xFC,
    IRKEY_TTX_SUBCODE   = 0x53,
    IRKEY_TTX_RED       = 0x47,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0x57,
    IRKEY_TTX_CYAN      = 0x5B,
    
    IRKEY_VOL_UP       = 0x83,
    IRKEY_VOL_DOWM     = 0x86,
    IRKEY_CH_UP        = 0x80,
    IRKEY_CH_DOWM      = 0x8E,
    
    IRKEY_MUTE         = 0xbb,//0x43,//0x05,
    IRKEY_MTS          = 0x41,
    IRKEY_DISPLAY      = 0xFB,  //Info
    IRKEY_SLEEP        = 0x42,
    IRKEY_ZOOM         = 0x43,//0x4C,  //
    
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0xFE,//no this key
    IRKEY_CECLink      = 0x5F,      //Clock Key
    IRKEY_PLAY         = 0x01,
    IRKEY_STOP         = 0x11,
    IRKEY_PAUSE        = 0x1D,
    IRKEY_DUMY         = 0xff
    
}IrCommandType;
#ifdef IR_MODE_SEL_2
typedef enum _IrCommandType2
{
    
    IRKEY_POWER_2        = 0x08,
    IRKEY_INPUT_2        = 0x0B, 
    
    IRKEY_OSD_MENU_2     = 0x43,
    IRKEY_OSD_LEFT_2     = 0x07,
    IRKEY_OSD_RIGHT_2    = 0x06,
    IRKEY_OSD_UP_2       = 0x40,
    IRKEY_OSD_DOWM_2     = 0x41,
    IRKEY_OSD_EXIT_2     = 0x5B,

    IRKEY_OSD_Enter_2  = 0x44,
    
    IRKEY_PIP_2          = 0xAB,//0x09,
    IRKEY_PIP_Position_2          = 0x39,   //
    IRKEY_PIP_Swap_2          = 0x30,
    IRKEY_PIP_AudioSwap_2          = 0x52,

    
    IRKEY_VOL_UP_2       = 0x02,//0x58,//IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM_2     = 0x03,//0x5D,//IRKEY_TTX_UPDATE,
    IRKEY_CH_UP_2        = 0x89,//IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM_2      = 0x19,//IRKEY_TTX_INDEX,
    
    IRKEY_MUTE_2         = 0x09,//0x05,
    IRKEY_SLEEP_2        = 0x0E,
    IRKEY_ZOOM_2         = 0x79,//0x4C,  //
    

}IrCommandType2;

#endif

#elif (defined(KunDe_Project))
typedef enum _IrCommandType
{
    IRKEY_0             = 0x51,
    IRKEY_1             = 0xaa,
    IRKEY_2             = 0xF9,
    IRKEY_3             = 0xFA,
    IRKEY_4             = 0x88,
    IRKEY_5             = 0xFE,//4C,
    IRKEY_6             = 0x4D,
    IRKEY_7             = 0x53,
    IRKEY_8             = 0x54,
    IRKEY_9             = 0x55,
    IRKEY_100           = 0xFF, //no this key
    
    IRKEY_POWER        = 0x20,
    IRKEY_INPUT        = 0xDB,  //source
    
    IRKEY_OSD_MENU     = 0x60,
    IRKEY_OSD_LEFT     = 0x64,
    IRKEY_OSD_RIGHT    = 0x65,
    IRKEY_OSD_UP       = 0x62,
    IRKEY_OSD_DOWM     = 0x63,
    IRKEY_OSD_EXIT     = 0xD4,

    IRKEY_OSD_Enter  = 0x61,
    
    IRKEY_PIP          = 0x11,//0x09,
    IRKEY_PIP_Position          = 0x0E,   //
    IRKEY_PIP_Swap          = 0x4E,
    IRKEY_PIP_AudioSwap          = 0x4A,

    IRKEY_HotBrightness          = 0x66,
    IRKEY_HotVolume          = 0x67,

    IRKEY_HotKey1          = 0x68,
    IRKEY_HotKey2          = 0x69,
	
    IRKEY_TTX_REVEAL    = 0x4F,
    IRKEY_TTX_UPDATE    = 0x0C,
    IRKEY_TTX_SIZE      = 0xFD,
    IRKEY_TTX_CANCEL      = 0x1B,   //Exit
    IRKEY_TTX_MIX       = 0x1C,
    IRKEY_TTX_INDEX     = 0x18,
    IRKEY_TTX_HOLD      = 0x00,
    IRKEY_TTX_LIST      = 0xFC,
    IRKEY_TTX_SUBCODE   = 0x53,
    IRKEY_TTX_RED       = 0x47,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0x57,
    IRKEY_TTX_CYAN      = 0x5B,
    
    IRKEY_VOL_UP       = 0x83,
    IRKEY_VOL_DOWM     = 0x86,
    IRKEY_CH_UP        = 0x80,
    IRKEY_CH_DOWM      = 0x8E,
    
    IRKEY_MUTE         = 0xDF,//0x05,
    IRKEY_MTS          = 0x41,
    IRKEY_DISPLAY      = 0xFB,  //Info
    IRKEY_SLEEP        = 0x42,
    IRKEY_ZOOM         = 0xC9,//0x4C,  //
    
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0xFE,//no this key
    IRKEY_CECLink      = 0x5F,      //Clock Key
    IRKEY_PLAY         = 0x01,
    IRKEY_STOP         = 0x11,
    IRKEY_PAUSE        = 0x1D,
    IRKEY_DUMY         = 0xff
    
}IrCommandType;


#ifdef IR_MODE_SEL_2
typedef enum _IrCommandType2
{
    
    IRKEY_POWER_2        = 0x12,
    IRKEY_INPUT_2        = 0x0B, 
    
    IRKEY_OSD_MENU_2     = 0x05,
    IRKEY_OSD_LEFT_2     = 0x41,
    IRKEY_OSD_RIGHT_2    = 0x40,
    IRKEY_OSD_UP_2       = 0x04,
    IRKEY_OSD_DOWM_2     = 0x06,
    IRKEY_OSD_EXIT_2     = 0x1B,

    IRKEY_OSD_Enter_2  = 0x1A,
    
    IRKEY_PIP_2          = 0xAB,//0x09,
    IRKEY_PIP_Position_2          = 0x39,   //
    IRKEY_PIP_Swap_2          = 0x30,
    IRKEY_PIP_AudioSwap_2          = 0x52,

    
    IRKEY_VOL_UP_2       = 0x02,//0x58,//IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM_2     = 0x03,//0x5D,//IRKEY_TTX_UPDATE,
    IRKEY_CH_UP_2        = 0x89,//IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM_2      = 0x19,//IRKEY_TTX_INDEX,
    
    IRKEY_MUTE_2         = 0x09,//0x05,
    IRKEY_SLEEP_2        = 0x0E,
    IRKEY_ZOOM_2         = 0x79,//0x4C,  //
    

}IrCommandType2;

#endif

#elif (defined(ZhongChuangLianHe_Project))
typedef enum _IrCommandType
{
    IRKEY_0             = 0x51,
    IRKEY_1             = 0xaa,
    IRKEY_2             = 0xF9,
    IRKEY_3             = 0xFA,
    IRKEY_4             = 0x88,
    IRKEY_5             = 0xFE,//4C,
    IRKEY_6             = 0x4D,
    IRKEY_7             = 0x53,
    IRKEY_8             = 0x54,
    IRKEY_9             = 0x55,
    IRKEY_100           = 0xFF, //no this key
    
    IRKEY_POWER        = 0x00,
    IRKEY_INPUT        = 0x10,  //source
    
    IRKEY_OSD_MENU     = 0x04,
    IRKEY_OSD_LEFT     = 0x08,
    IRKEY_OSD_RIGHT    = 0x0A,
    IRKEY_OSD_UP       = 0x05,
    IRKEY_OSD_DOWM     = 0x0D,
    IRKEY_OSD_EXIT     = 0x0E,

    IRKEY_OSD_Enter  = 0x09,
    
    IRKEY_PIP          = 0x22,//0x09,
    IRKEY_PIP_Position          = 0x23,   //
    IRKEY_PIP_Swap          = 0x24,
    IRKEY_PIP_AudioSwap          = 0x25,

	
    IRKEY_TTX_REVEAL    = 0x4F,
    IRKEY_TTX_UPDATE    = 0x0C,
    IRKEY_TTX_SIZE      = 0xFD,
    IRKEY_TTX_CANCEL      = 0x1B,   //Exit
    IRKEY_TTX_MIX       = 0x1C,
    IRKEY_TTX_INDEX     = 0x18,
    IRKEY_TTX_HOLD      = 0x00,
    IRKEY_TTX_LIST      = 0xFC,
    IRKEY_TTX_SUBCODE   = 0x53,
    IRKEY_TTX_RED       = 0x47,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0x57,
    IRKEY_TTX_CYAN      = 0x5B,
    
    IRKEY_VOL_UP       = 0x11,
    IRKEY_VOL_DOWM     = 0x12,
    IRKEY_CH_UP        = 0xF6,
    IRKEY_CH_DOWM      = 0xF7,
    
    IRKEY_MUTE         = 0x02,//0x05,
    IRKEY_MTS          = 0x41,
    IRKEY_DISPLAY      = 0xFB,  //Info
    IRKEY_SLEEP        = 0x42,
    IRKEY_ZOOM         = 0xDB,//0x4C,  //
    
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0xFE,//no this key
    IRKEY_CECLink      = 0x5F,      //Clock Key
    IRKEY_PLAY         = 0x01,
    IRKEY_STOP         = 0x11,
    IRKEY_PAUSE        = 0x1D,
    IRKEY_DUMY         = 0xff
    
}IrCommandType;


#elif (defined(Changjia_Project))

typedef enum _IrCommandType
{
    IRKEY_0             = 0x51,
    IRKEY_1             = 0xaa,
    IRKEY_2             = 0xF9,
    IRKEY_3             = 0xFA,
    IRKEY_4             = 0x88,
    IRKEY_5             = 0xFE,//4C,
    IRKEY_6             = 0x4D,
    IRKEY_7             = 0x53,
    IRKEY_8             = 0x54,
    IRKEY_9             = 0x55,
    IRKEY_100           = 0xFF, //no this key
    
    IRKEY_POWER        = 0x03,
    IRKEY_INPUT        = 0x1E,  //source
    
    IRKEY_OSD_MENU     = 0x1C,
    IRKEY_OSD_LEFT     = 0x40,
    IRKEY_OSD_RIGHT    = 0x42,
    IRKEY_OSD_UP       = 0x1D,
    IRKEY_OSD_DOWM     = 0x45,
    IRKEY_OSD_EXIT     = 0x44,

    IRKEY_OSD_Enter  = 0x41,
    
    IRKEY_PIP          = 0x0A,//0x09,
    IRKEY_PIP_Position          = 0x60,   //
    IRKEY_PIP_Swap          = 0x4E,
    IRKEY_PIP_AudioSwap          = 0x4A,

	
    IRKEY_TTX_REVEAL    = 0x4F,
    IRKEY_TTX_UPDATE    = 0x61,
    IRKEY_TTX_SIZE      = 0xFD,
    IRKEY_TTX_CANCEL      = 0x1B,   //Exit
    IRKEY_TTX_MIX       = 0x1C,
    IRKEY_TTX_INDEX     = 0x18,
    IRKEY_TTX_HOLD      = 0x00,
    IRKEY_TTX_LIST      = 0xFC,
    IRKEY_TTX_SUBCODE   = 0x53,
    IRKEY_TTX_RED       = 0x47,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0x57,
    IRKEY_TTX_CYAN      = 0x5B,
    
    IRKEY_VOL_UP       = 0x16,
    IRKEY_VOL_DOWM     = 0x1A,
    IRKEY_CH_UP        = 0x14,
    IRKEY_CH_DOWM      = 0x18,
    
    IRKEY_MUTE         = 0x15,
    IRKEY_MTS          = 0x71,
    IRKEY_DISPLAY      = 0xFB,  //Info
    IRKEY_SLEEP        = 0x72,
    IRKEY_ZOOM         = 0x76,//0x4C,  //
    
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0xFE,//no this key
    IRKEY_CECLink      = 0x5F,      //Clock Key
    IRKEY_PLAY         = 0x01,
    IRKEY_STOP         = 0x63,
    IRKEY_PAUSE        = 0x1D,
    IRKEY_DUMY         = 0xff
    
}IrCommandType;

#elif (defined(MingCai_Project))

typedef enum _IrCommandType
{
    IRKEY_0             = 0x51,
    IRKEY_1             = 0xaa,
    IRKEY_2             = 0xF9,
    IRKEY_3             = 0xFA,
    IRKEY_4             = 0x88,
    IRKEY_5             = 0xFE,//4C,
    IRKEY_6             = 0x4D,
    IRKEY_7             = 0xF2,
    IRKEY_8             = 0x54,
    IRKEY_9             = 0x55,
    IRKEY_100           = 0xFF, //no this key
    
    IRKEY_POWER        = 0x0B,
    IRKEY_INPUT        = 0x1F,
    
    IRKEY_OSD_MENU     =0x48,
    IRKEY_OSD_LEFT     = 0x1A,
    IRKEY_OSD_RIGHT    = 0x40,
    IRKEY_OSD_UP       = 0x1C,
    IRKEY_OSD_DOWM     = 0x19,
    IRKEY_OSD_EXIT     = 0x56,

    IRKEY_OSD_Enter  = 0x59,
    
    IRKEY_PIP          = 0xD1,//0x09,
    IRKEY_PIP_Position          = 0x0E,   //
    IRKEY_PIP_Swap          = 0xDA,
    IRKEY_PIP_AudioSwap          = 0x4A,

	
    IRKEY_TTX_REVEAL    = 0xED,
    IRKEY_TTX_UPDATE    = 0xDB,
    IRKEY_TTX_SIZE      = 0xFD,
    IRKEY_TTX_CANCEL      = 0x1B,   //Exit
    IRKEY_TTX_MIX       = 0x1C,
    IRKEY_TTX_INDEX     = 0x18,
    IRKEY_TTX_HOLD      = 0x00,
    IRKEY_TTX_LIST      = 0xFC,
    IRKEY_TTX_SUBCODE   = 0xF1,
    IRKEY_TTX_RED       = 0x47,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0xF4,
    IRKEY_TTX_CYAN      = 0x5B,
    
    IRKEY_VOL_UP       = 0x4F,
    IRKEY_VOL_DOWM     = 0x57,
    IRKEY_CH_UP        = 0x80,
    IRKEY_CH_DOWM      = 0x8E,
    
    IRKEY_MUTE         = 0x0C,
    IRKEY_MTS          = 0x41,
    IRKEY_DISPLAY      = 0xFB,  //Info
    IRKEY_SLEEP        = 0x42,
    IRKEY_ZOOM         = 0x43,//0x4C,  //
    
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0xFE,//no this key
    IRKEY_CECLink      = 0x5F,      //Clock Key
    IRKEY_PLAY         = 0x01,
    IRKEY_STOP         = 0xDD,
    IRKEY_PAUSE        = 0x1D,
    IRKEY_DUMY         = 0xff
    
}IrCommandType;

#elif (defined(XuLi_Project))

typedef enum _IrCommandType
{
    IRKEY_0             = 0x51,
    IRKEY_1             = 0xaa,
    IRKEY_2             = 0xF9,
    IRKEY_3             = 0xFA,
    IRKEY_4             = 0x88,
    IRKEY_5             = 0xFE,//4C,
    IRKEY_6             = 0x4D,
    IRKEY_7             = 0xF2,
    IRKEY_8             = 0x54,
    IRKEY_9             = 0x55,
    IRKEY_100           = 0xFF, //no this key
    
    IRKEY_POWER        = 0x57,
    IRKEY_INPUT        = 0x14,
    
    IRKEY_OSD_MENU     =0x19,
    IRKEY_OSD_LEFT     = 0x53,
    IRKEY_OSD_RIGHT    = 0x10,
    IRKEY_OSD_UP       = 0x0D,
    IRKEY_OSD_DOWM     = 0x4F,
    IRKEY_OSD_EXIT     = 0x56,

    IRKEY_OSD_Enter  = 0x11,
    
    IRKEY_PIP          = 0xD1,//0x09,
    IRKEY_PIP_Position          = 0x0E,   //
    IRKEY_PIP_Swap          = 0xDA,
    IRKEY_PIP_AudioSwap          = 0x4A,

	
    IRKEY_TTX_REVEAL    = 0xED,
    IRKEY_TTX_UPDATE    = 0xDB,
    IRKEY_TTX_SIZE      = 0xFD,
    IRKEY_TTX_CANCEL      = 0x1B,   //Exit
    IRKEY_TTX_MIX       = 0x1C,
    IRKEY_TTX_INDEX     = 0x18,
    IRKEY_TTX_HOLD      = 0x00,
    IRKEY_TTX_LIST      = 0xFC,
    IRKEY_TTX_SUBCODE   = 0xF1,
    IRKEY_TTX_RED       = 0x47,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0xF4,
    IRKEY_TTX_CYAN      = 0x5B,
    
    IRKEY_VOL_UP       = 0x0C,
    IRKEY_VOL_DOWM     = 0x4E,
    IRKEY_CH_UP        = 0x80,
    IRKEY_CH_DOWM      = 0x8E,
    
    IRKEY_MUTE         = 0x0F,
    IRKEY_MTS          = 0x41,
    IRKEY_DISPLAY      = 0xFB,  //Info
    IRKEY_SLEEP        = 0x42,
    IRKEY_ZOOM         = 0x43,//0x4C,  //
    
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0xFE,//no this key
    IRKEY_CECLink      = 0x5F,      //Clock Key
    IRKEY_PLAY         = 0x01,
    IRKEY_STOP         = 0xDD,
    IRKEY_PAUSE        = 0x1D,
    IRKEY_DUMY         = 0xff
    
}IrCommandType;
/*
#elif LHCX_IR_Function_Enable

typedef enum _IrCommandType
{
    IRKEY_0             = 0x51,
    IRKEY_1             = 0xaa,
    IRKEY_2             = 0xF9,
    IRKEY_3             = 0xFA,
    IRKEY_4             = 0x88,
    IRKEY_5             = 0xFE,//4C,
    IRKEY_6             = 0x4D,
    IRKEY_7             = 0x53,
    IRKEY_8             = 0x54,
    IRKEY_9             = 0x55,
    IRKEY_100           = 0xFF, //no this key
    
    IRKEY_POWER        = 0x5C,
    IRKEY_INPUT        = 0x1E,  //source
    
    IRKEY_OSD_MENU     = 0x4C,
    IRKEY_OSD_LEFT     = 0x0F,
    IRKEY_OSD_RIGHT    = 0x07,
    IRKEY_OSD_UP       = 0x48,
    IRKEY_OSD_DOWM     = 0x0A,
    IRKEY_OSD_EXIT     = 0x44,

    IRKEY_OSD_Enter  = 0x0B,
    
    IRKEY_PIP          = 0x11,//0x09,
    IRKEY_PIP_Position          = 0x0E,   //
    IRKEY_PIP_Swap          = 0x4E,
    IRKEY_PIP_AudioSwap          = 0x4A,

	
    IRKEY_TTX_REVEAL    = 0x4F,
    IRKEY_TTX_UPDATE    = 0x0C,
    IRKEY_TTX_SIZE      = 0xFD,
    IRKEY_TTX_CANCEL      = 0x1B,   //Exit
    IRKEY_TTX_MIX       = 0x1C,
    IRKEY_TTX_INDEX     = 0x18,
    IRKEY_TTX_HOLD      = 0x00,
    IRKEY_TTX_LIST      = 0xFC,
    IRKEY_TTX_SUBCODE   = 0x53,
    IRKEY_TTX_RED       = 0x47,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0x57,
    IRKEY_TTX_CYAN      = 0x5B,
    
    IRKEY_VOL_UP       = 0x10,//0x58,//IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM     = 0x1F,//0x5D,//IRKEY_TTX_UPDATE,
    IRKEY_CH_UP        = 0x89,//IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM      = 0x19,//IRKEY_TTX_INDEX,
    
    IRKEY_MUTE         = 0x14,//0x05,
    IRKEY_MTS          = 0x41,
    IRKEY_DISPLAY      = 0xFB,  //Info
    IRKEY_SLEEP        = 0x42,
    IRKEY_ZOOM         = 0x46,//0x4C,  //
    
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0xFE,//no this key
    IRKEY_CECLink      = 0x5F,      //Clock Key
    IRKEY_PLAY         = 0x01,
    IRKEY_STOP         = 0x11,
    IRKEY_PAUSE        = 0x1D,
    IRKEY_DUMY         = 0xff
    
}IrCommandType;

#ifdef IR_MODE_SEL_2
typedef enum _IrCommandType2
{
    
    IRKEY_POWER_2        = 0x08,
    IRKEY_INPUT_2        = 0x0B, 
    
    IRKEY_OSD_MENU_2     = 0x43,
    IRKEY_OSD_LEFT_2     = 0x07,
    IRKEY_OSD_RIGHT_2    = 0x06,
    IRKEY_OSD_UP_2       = 0x40,
    IRKEY_OSD_DOWM_2     = 0x41,
    IRKEY_OSD_EXIT_2     = 0x5B,

    IRKEY_OSD_Enter_2  = 0x44,
    
    IRKEY_PIP_2          = 0xAB,//0x09,
    IRKEY_PIP_Position_2          = 0x39,   //
    IRKEY_PIP_Swap_2          = 0x30,
    IRKEY_PIP_AudioSwap_2          = 0x52,

    
    IRKEY_VOL_UP_2       = 0x02,//0x58,//IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM_2     = 0x03,//0x5D,//IRKEY_TTX_UPDATE,
    IRKEY_CH_UP_2        = 0x89,//IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM_2      = 0x19,//IRKEY_TTX_INDEX,
    
    IRKEY_MUTE_2         = 0x09,//0x05,
    IRKEY_SLEEP_2        = 0x0E,
    IRKEY_ZOOM_2         = 0x79,//0x4C,  //
    

}IrCommandType2;

#endif
*/



#else


typedef enum _IrCommandType
{
    IRKEY_0             = 0x51,
    IRKEY_1             = 0x1C,
    IRKEY_2             = 0x1E,
    IRKEY_3             = 0x4C,
    IRKEY_4             = 0x1B,
    IRKEY_5             = 0xFE,//4C,
    IRKEY_6             = 0x4D,
    IRKEY_7             = 0x53,
    IRKEY_8             = 0x54,
    IRKEY_9             = 0x55,

    IRKEY_100           = 0xFF, //no this key

    IRKEY_POWER        = 0x0C,
    IRKEY_INPUT        = 0x0D,  //source

    IRKEY_OSD_MENU     = 0x0F,
    IRKEY_OSD_LEFT     = 0x16,
    IRKEY_OSD_RIGHT    = 0x15,
    IRKEY_OSD_UP       = 0x17,
    IRKEY_OSD_DOWM     = 0x18,
    IRKEY_OSD_EXIT     = 0x14,

    IRKEY_PIP          = 0x40,
    IRKEY_TTX_REVEAL    = 0x4F,
    IRKEY_TTX_UPDATE    = 0x0C,
    IRKEY_TTX_SIZE      = 0x43,
    IRKEY_TTX_CANCEL      = 0x1B,   //Exit

    IRKEY_TTX_MIX       = 0x1C,
    IRKEY_TTX_INDEX     = 0x18,
    IRKEY_TTX_HOLD      = 0x00,
    IRKEY_TTX_LIST      = 0x10,
    IRKEY_TTX_SUBCODE   = 0x53,

    IRKEY_TTX_RED       = 0x47,
    IRKEY_TTX_GREEN     = 0x4B,
    IRKEY_TTX_YELLOW    = 0x57,
    IRKEY_TTX_CYAN      = 0x5B,

    IRKEY_VOL_UP       = 0x16,//IRKEY_TTX_SIZE,
    IRKEY_VOL_DOWM     = 0x15,//IRKEY_TTX_UPDATE,
    IRKEY_CH_UP        = 0x1F,//IRKEY_TTX_HOLD,
    IRKEY_CH_DOWM      = 0x19,//IRKEY_TTX_INDEX,

    IRKEY_MUTE         = 0x5A,
    IRKEY_MTS          = 0x41,
    IRKEY_DISPLAY      = 0x14,  //Info
    IRKEY_SLEEP        = 0x45,
    IRKEY_ZOOM         = 0x4C,  //
    IRKEY_PICMODE      = IRKEY_TTX_GREEN,
    IRKEY_SOUNDMODE    = IRKEY_TTX_YELLOW,
    IRKEY_SCANMODE     = IRKEY_TTX_CYAN,
    IRKEY_CAPTURE      = 0xFE,//no this key


    IRKEY_CECLink      = 0x5F,      //Clock Key
    IRKEY_PLAY         = 0x01,
    IRKEY_STOP         = 0x11,
    IRKEY_PAUSE        = 0x1D,

    IRKEY_DUMY         = 0xff
}IrCommandType;

#endif



#endif
typedef enum _IrControlMenuType
{
    IR_MENU_CHANNEL1,
    IR_MENU_CHANNEL2,

    IR_MENU_NULL
}IrControlMenuTyp;

///////////////////////////////////////////////////
// Subroutines
///////////////////////////////////////////////////
_IRFUNCDEC_ void irDecodeCommand(void);
//_IRFUNCDEC_ void irAdjustNumKey(BYTE ucPressNum);
//_IRFUNCDEC_ void irPullFactoryKey(BYTE ucPressKey);
_IRFUNCDEC_ void irDecodeNumKey(void);

#endif  //(IR_REMOTE_SEL != NO_REMOTE)
#endif

