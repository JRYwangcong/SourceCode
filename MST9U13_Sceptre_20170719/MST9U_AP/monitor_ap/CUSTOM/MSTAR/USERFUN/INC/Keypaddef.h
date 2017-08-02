#ifndef _KEYPADDEF_H
#define _KEYPADDEF_H

typedef enum
{
  KEY_POWER     =BIT0, // power
  KEY_MENU      =BIT1, // menu
  KEY_PLUS      =BIT2, // right/up
  KEY_MINUS     =BIT3, // left/down
  KEY_EXIT      =BIT4, // tick

  KEY_INCVALUE =BIT5,
  KEY_DECVALUE =BIT6,
  
  KEY_FACTORY   =KEY_POWER|KEY_MENU,
  KEY_DIAG      =KEY_MENU|KEY_PLUS,

  KEY_NOTHING   =0
} KeypadMaskType;

/// DONOT CHANGE THE DEFINITIONS OF ButtonType ~~~~~~~~~~~~~~~~~~~~~~~~~
typedef enum
{ BTN_PLUS,
  BTN_MINUS,
  BTN_MENU,
  BTN_EXIT,
  BTN_POWER,

  BTN_INCVALUE,
  BTN_DECVALUE,

  BTN_END_BTN,
  BTN_REPEAT,

  BTN_NOTHING,

} ButtonType;
///~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//#define KEYPAD_MASK (KEY_POWER|KEY_PLUS|KEY_MINUS|KEY_MENU|KEY_EXIT)
//#define KEYPAD_MASK (KEY_POWER|KEY_PLUS|KEY_MINUS|KEY_MENU)
#define KEYPAD_MASK (KEY_POWER|KEY_PLUS|KEY_MINUS|KEY_MENU|KEY_EXIT|KEY_INCVALUE|KEY_DECVALUE)


typedef enum
{
    VK_NOTHING,
    VK_UP,
    VK_DOWN,
    VK_LEFT,
    VK_RIGHT,
    VK_MENU,
    VK_SOURCE,
    VK_PIP,
    VK_POWER,
    VK_EXIT,

    VK_VOL_UP,
    VK_VOL_DOWN,
    
    VK_BRI_UP,
    VK_BRI_DOWN,
   
    VK_REPEAT,
    
}VKeyType;


#endif /// _KEYPADDEF_H
