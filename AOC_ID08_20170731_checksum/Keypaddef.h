#if Customer
typedef enum
{ KEY_POWER	=BIT0, // power
  KEY_PLUS	=BIT2, // right
  KEY_MINUS	=BIT3, // left
  KEY_MENU	=BIT1, // menu
  KEY_EXIT	=BIT4, // exit/auto
  KEY_FACTORY	=KEY_POWER|KEY_EXIT|KEY_MENU,
  KEY_ISP	=KEY_PLUS|KEY_MINUS,

  KEY_NOTHING	=0
} KeypadMaskType;
#elif MST2Layer
typedef enum
{ KEY_POWER	=BIT0,
  KEY_PLUS	=BIT4,
  KEY_MINUS	=BIT3,
  KEY_MENU	=BIT2,
  KEY_EXIT	=BIT1,
  KEY_FACTORY	=KEY_POWER|KEY_EXIT|KEY_MENU,
  KEY_ISP	=KEY_PLUS|KEY_MINUS,

  KEY_NOTHING	=0
} KeypadMaskType;
#elif AOCKey
typedef enum
{ KEY_POWER =BIT0, // power
  KEY_PLUS	 =BIT2,//BIT2, // right
  KEY_MINUS =BIT3,//BIT3, // left
  KEY_MENU	 =BIT1,//BIT1, // menu
  KEY_EXIT	 =BIT4,//BIT4, // exit/auto
  KEY_SELECT  =BIT6, //2006-02-22  source select 
  
#if ScanKey//petit 20130102
  KEY_FACTORY	= KEY_POWER|KEY_PLUS|KEY_MINUS,
#else
  KEY_FACTORY	= KEY_POWER|KEY_PLUS,//KEY_MENU,//KEY_POWER|KEY_EXIT|KEY_MENU,
#endif  
  //KEY_ISP		=KEY_PLUS|KEY_MINUS,//KEY_MENU|KEY_EXIT,
  KEY_PRODUCT	=KEY_PLUS|KEY_MINUS,
  #if AllInOneKey
  KEY_LOCK		=KEY_POWER|KEY_MINUS,
  #else
  KEY_LOCK		=KEY_POWER|KEY_MINUS,//=KEY_POWER|KEY_MENU,
  #endif
  KEY_MUTE		=KEY_EXIT|KEY_MENU,
  #if EnableVcomAdjustItem
  KEY_VCOM		=KEY_MINUS|KEY_MENU,
  #endif
  KEY_NOTHING	=0
} KeypadMaskType;

#else
typedef enum
{ KEY_POWER	=BIT2,
  KEY_PLUS	=BIT4,
  KEY_MINUS	=BIT3,
  KEY_MENU	=BIT5,
  KEY_EXIT	=BIT6,
  KEY_FACTORY	=KEY_POWER|KEY_EXIT|KEY_MENU,
  KEY_ISP	=KEY_PLUS|KEY_MINUS,

  KEY_NOTHING	=0
} KeypadMaskType;
#endif

typedef enum
{ BTN_Plus,  // 4
  BTN_Minus,//8
  BTN_Menu,// 2
  BTN_Exit, // 16
  BTN_Power, // 1
  BTN_Select,
  
  BTN_EndBTN,
  BTN_Repeat,

  BTN_Power1,
  #if EnableVcomAdjustItem
  BTN_VCOM,
  #endif
  BTN_Nothing
} ButtonType;

//2006-02-22 #define KeypadMask (KEY_POWER|KEY_PLUS|KEY_MINUS|KEY_MENU|KEY_EXIT)
#define KeypadMask (KEY_POWER|KEY_PLUS|KEY_MINUS|KEY_MENU|KEY_EXIT|KEY_SELECT)
