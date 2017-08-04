#include <stdlib.h>
#include "board.h"
#include "types.h"
#include "ms_rwreg.h"
#include "ms_reg.h"
#include "msosd.h"
#include "extlib.h"
#include "global.h"
#include "Menudef.h"
#include "MainMonoFont.h"
#include "MainCompFont.h"
#include "debug.h"

#if 0
void Osd_LoadMainMonoFont(BYTE addr, BYTE *fontPtr, WORD num)
{
    mStar_WriteByte(RAMFA, addr);
    while (num--)
    {
        mStar_WriteByte(RAMFD, *(fontPtr++));
    }
}
#endif

#if LANGUAGE_TYPE== All14Language_AddJap
extern void LoadMainTitleFont_1(BYTE index);
#endif


void LoadMainTitleFont(BYTE index)
{

 switch(UserPrefLanguage)
 {


	case LANG_English:
#ifdef  DEBUG_EN
       case LANG_Russia:
#endif
		if(index==0)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_English1,0); // load osd fonts
		else if(index==1)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_English2,0); // load osd fonts
		else if(index==2)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_English3,0); // load osd fonts
		else if(index==3)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_English4,0); // load osd fonts
		else if(index==4)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_English5,0); // load osd fonts
		else if(index==5)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_English6,0); // load osd fonts
		else if(index==6)
			#if CenterTitleText
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_All7,0); // load osd fonts			
			#else
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_English7,0); // load osd fonts
			#endif
		else if(index==7)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_English8,0); // load osd fonts
		else
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_English9,0); // load osd fonts

		break;

	case  LANG_Spanish:
		if(index==0)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Spanish1,0); // load osd fonts
		else if(index==1)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Spanish2,0); // load osd fonts
		else if(index==2)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Spanish3,0); // load osd fonts
		else if(index==3)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Spanish4,0); // load osd fonts
		else if(index==4)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Spanish5,0); // load osd fonts
		else if(index==5)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Spanish6,0); // load osd fonts
		else if(index==6)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_All7,0); // load osd fonts
		else if(index==7)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Spanish8,0); // load osd fonts
		else
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Spanish9,0); // load osd fonts
		break;

	case  LANG_France:
		if(index==0)
         			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_France1,0); // load osd fonts
		else if(index==1)
       			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_France2,0); // load osd fonts
		else if(index==2)
        			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_France3,0); // load osd fonts
		else if(index==3)
      			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_France4,0); // load osd fonts
		else if(index==4)
  			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_France5,0); // load osd fonts
		else if(index==5)
        			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_France6,0); // load osd fonts
		else if(index==6)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_All7,0); // load osd fonts
		else if(index==7)
       	        mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_France8,0); // load osd fonts
		else
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_France9,0); // load osd fonts
		break;

#ifndef DEBUG_EN
	case  LANG_Russia:
		if(index==0)
      			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Russian1,0); // load osd fonts
		else if(index==1)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Russian2,0); // load osd fonts
		else if(index==2)
       	        mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Russian3,0); // load osd fonts
		else if(index==3)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Russian4,0); // load osd fonts
		else if(index==4)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Russian5,0); // load osd fonts
		else if(index==5)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Russian6,0); // load osd fonts
		else if(index==6)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Russian7,0); // load osd fonts
		else if(index==7)
      			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Russian8,0); // load osd fonts
		else
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Russian9,0); // load osd fonts
		break;
#endif

#if LANGUAGE_TYPE== ASIA

	case  LANG_Portugues:
		if(index==0)
      			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues1,0); // load osd fonts
		else if(index==1)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues2,0); // load osd fonts
		else if(index==2)
      			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues3,0); // load osd fonts
		else if(index==3)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues4,0); // load osd fonts
		else if(index==4)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues5,0); // load osd fonts
		else if(index==5)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues6,0); // load osd fonts
		else if(index==6)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues7,0); // load osd fonts
		else if(index==7)
    			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues8,0); // load osd fonts
		else
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues9,0); // load osd fonts
		break;

#if CenterTitleText
	case  LANG_SChina:
		if(index==0)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina1,0); // load osd fonts	
		else if(index==1)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina2,0); // load osd fonts	
		else if(index==2)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina3,0); // load osd fonts	
		else if(index==3)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina4,0); // load osd fonts	
		else if(index==4)
         	mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina5,0); // load osd fonts	
		else if(index==5)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina6,0); // load osd fonts	
		else if(index==6)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina7,0); // load osd fonts	
		else if(index==7)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina8,0); // load osd fonts	
		else
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina9,0); // load osd fonts	
		break;
	case  LANG_TChina:
         if(index==0)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina1,0); // load osd fonts	
         else if(index==1)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina2,0); // load osd fonts	
         else if(index==2)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina3,0); // load osd fonts	
         else if(index==3)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina4,0); // load osd fonts	
         else if(index==4)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina5,0); // load osd fonts	
         else if(index==5)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina6,0); // load osd fonts	
         else if(index==6)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina7,0); // load osd fonts	
         else if(index==7)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina8,0); // load osd fonts	
         else
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina9,0); // load osd fonts	
         break;
	case  LANG_Korea:
         if(index==0)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea1,0); // load osd fonts	
         else if(index==1)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea2,0); // load osd fonts	
         else if(index==2)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea3,0); // load osd fonts	
         else if(index==3)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea4,0); // load osd fonts	
         else if(index==4)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea5,0); // load osd fonts	
         else if(index==5)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea6,0); // load osd fonts	
         else if(index==6)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea7,0); // load osd fonts	
         else if(index==7)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea8,0); // load osd fonts	
         else
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea9,0); // load osd fonts	
         break;
#else
	case  LANG_SChina:
		if(index==0)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina1, sizeof(tMainFont_SChina1));
		else if(index==1)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina2, sizeof(tMainFont_SChina2));
		else if(index==2)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina3, sizeof(tMainFont_SChina3));
		else if(index==3)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina4, sizeof(tMainFont_SChina4));
		else if(index==4)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina5, sizeof(tMainFont_SChina5));
		else if(index==5)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina6, sizeof(tMainFont_SChina6));
		else if(index==6)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina7, sizeof(tMainFont_SChina7));
		else if(index==7)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina8, sizeof(tMainFont_SChina8));
		else
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina9, sizeof(tMainFont_SChina9));
		break;

	case  LANG_TChina:
         if(index==0)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina1, sizeof(tMainFont_TChina1));
         else if(index==1)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina2, sizeof(tMainFont_TChina2));
         else if(index==2)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina3, sizeof(tMainFont_TChina3));
         else if(index==3)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina4, sizeof(tMainFont_TChina4));
         else if(index==4)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina5, sizeof(tMainFont_TChina5));
         else if(index==5)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina6, sizeof(tMainFont_TChina6));
         else if(index==6)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina7, sizeof(tMainFont_TChina7));
         else if(index==7)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina8, sizeof(tMainFont_TChina8));
         else
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina9, sizeof(tMainFont_TChina9));
         break;

	case  LANG_Korea:
         if(index==0)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea1, sizeof(tMainFont_Korea1));
         else if(index==1)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea2, sizeof(tMainFont_Korea2));
         else if(index==2)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea3, sizeof(tMainFont_Korea3));
         else if(index==3)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea4, sizeof(tMainFont_Korea4));
         else if(index==4)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea5, sizeof(tMainFont_Korea5));
         else if(index==5)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea6, sizeof(tMainFont_Korea6));
         else if(index==6)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea7, sizeof(tMainFont_Korea7));
         else if(index==7)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea8, sizeof(tMainFont_Korea8));
         else
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea9, sizeof(tMainFont_Korea9));
         break;
#endif//CenterTitleText

#elif LANGUAGE_TYPE== All13Language

case  LANG_Portugues:
		if(index==0)
      			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues1,0); // load osd fonts
		else if(index==1)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues2,0); // load osd fonts
		else if(index==2)
      			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues3,0); // load osd fonts
		else if(index==3)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues4,0); // load osd fonts
		else if(index==4)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues5,0); // load osd fonts
		else if(index==5)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues6,0); // load osd fonts
		else if(index==6)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues7,0); // load osd fonts
		else if(index==7)
    			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues8,0); // load osd fonts
		else
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues9,0); // load osd fonts
		break;	
#if CenterTitleText
	case  LANG_SChina:
		if(index==0)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina1,0); // load osd fonts	
		else if(index==1)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina2,0); // load osd fonts	
		else if(index==2)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina3,0); // load osd fonts	
		else if(index==3)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina4,0); // load osd fonts	
		else if(index==4)
         	mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina5,0); // load osd fonts	
		else if(index==5)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina6,0); // load osd fonts	
		else if(index==6)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina7,0); // load osd fonts	
		else if(index==7)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina8,0); // load osd fonts	
		else
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina9,0); // load osd fonts	
		break;
	case  LANG_TChina:
         if(index==0)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina1,0); // load osd fonts	
         else if(index==1)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina2,0); // load osd fonts	
         else if(index==2)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina3,0); // load osd fonts	
         else if(index==3)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina4,0); // load osd fonts	
         else if(index==4)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina5,0); // load osd fonts	
         else if(index==5)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina6,0); // load osd fonts	
         else if(index==6)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina7,0); // load osd fonts	
         else if(index==7)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina8,0); // load osd fonts	
         else
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_TChina9,0); // load osd fonts	
         break;
	case  LANG_Korea:
         if(index==0)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea1,0); // load osd fonts	
         else if(index==1)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea2,0); // load osd fonts	
         else if(index==2)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea3,0); // load osd fonts	
         else if(index==3)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea4,0); // load osd fonts	
         else if(index==4)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea5,0); // load osd fonts	
         else if(index==5)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea6,0); // load osd fonts	
         else if(index==6)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea7,0); // load osd fonts	
         else if(index==7)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea8,0); // load osd fonts	
         else
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Korea9,0); // load osd fonts	
         break;
#else
	case  LANG_SChina:
		if(index==0)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina1, sizeof(tMainFont_SChina1));
		else if(index==1)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina2, sizeof(tMainFont_SChina2));
		else if(index==2)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina3, sizeof(tMainFont_SChina3));
		else if(index==3)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina4, sizeof(tMainFont_SChina4));
		else if(index==4)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina5, sizeof(tMainFont_SChina5));
		else if(index==5)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina6, sizeof(tMainFont_SChina6));
		else if(index==6)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina7, sizeof(tMainFont_SChina7));
		else if(index==7)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina8, sizeof(tMainFont_SChina8));
		else
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina9, sizeof(tMainFont_SChina9));
		break;

	case  LANG_TChina:
         if(index==0)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina1, sizeof(tMainFont_TChina1));
         else if(index==1)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina2, sizeof(tMainFont_TChina2));
         else if(index==2)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina3, sizeof(tMainFont_TChina3));
         else if(index==3)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina4, sizeof(tMainFont_TChina4));
         else if(index==4)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina5, sizeof(tMainFont_TChina5));
         else if(index==5)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina6, sizeof(tMainFont_TChina6));
         else if(index==6)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina7, sizeof(tMainFont_TChina7));
         else if(index==7)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina8, sizeof(tMainFont_TChina8));
         else
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_TChina9, sizeof(tMainFont_TChina9));
         break;

	case  LANG_Korea:
         if(index==0)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea1, sizeof(tMainFont_Korea1));
         else if(index==1)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea2, sizeof(tMainFont_Korea2));
         else if(index==2)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea3, sizeof(tMainFont_Korea3));
         else if(index==3)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea4, sizeof(tMainFont_Korea4));
         else if(index==4)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea5, sizeof(tMainFont_Korea5));
         else if(index==5)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea6, sizeof(tMainFont_Korea6));
         else if(index==6)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea7, sizeof(tMainFont_Korea7));
         else if(index==7)
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea8, sizeof(tMainFont_Korea8));
         else
             Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Korea9, sizeof(tMainFont_Korea9));
         break;
#endif//CenterTitleText
case  LANG_German:
		if(index==0)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German1,0); // load osd fonts
		else if(index==1)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German2,0); // load osd fonts
		else if(index==2)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German3,0); // load osd fonts
		else if(index==3)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German4,0); // load osd fonts
		else if(index==4)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German5,0); // load osd fonts
		else if(index==5)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German6,0); // load osd fonts
		else if(index==6)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_All7,0); // load osd fonts
		else if(index==7)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German8,0); // load osd fonts
		else
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German9,0); // load osd fonts
		break;
	break;

	case  LANG_Italian:
		if(index==0)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian1,0); // load osd fonts
		else if(index==1)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian2,0); // load osd fonts
		else if(index==2)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian3,0); // load osd fonts
		else if(index==3)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian4,0); // load osd fonts
		else if(index==4)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian5,0); // load osd fonts
		else if(index==5)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian6,0); // load osd fonts
		else if(index==6)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_All7,0); // load osd fonts
		else if(index==7)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian8,0); // load osd fonts
		else
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian9,0); // load osd fonts
		break;
       break;
	   
	   case  LANG_Swedish:
			   if(index==0)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish1,0); // load osd fonts
			   else if(index==1)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish2,0); // load osd fonts
			   else if(index==2)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish3,0); // load osd fonts
			   else if(index==3)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish4,0); // load osd fonts
			   else if(index==4)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish5,0); // load osd fonts
			   else if(index==5)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish6,0); // load osd fonts
			   else if(index==6)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_All7,0); // load osd fonts
			   else if(index==7)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish8,0); // load osd fonts
			   else
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish9,0); // load osd fonts
			   break;
		   break;
	   
		   case  LANG_Polish:
			   if(index==0)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish1,0); // load osd fonts
			   else if(index==1)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish2,0); // load osd fonts
			   else if(index==2)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish3,0); // load osd fonts
			   else if(index==3)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish4,0); // load osd fonts
			   else if(index==4)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish5,0); // load osd fonts
			   else if(index==5)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish6,0); // load osd fonts
			   else if(index==6)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish7,0); // load osd fonts
			   else if(index==7)
			#if CenterTitleText
					   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_English8,0); // load osd fonts
			#else
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish8,0); // load osd fonts
			#endif
			   else
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish9,0); // load osd fonts
			   break;
		   break;
	   
		   case  LANG_Czech:
			   if(index==0)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech1,0); // load osd fonts
			   else if(index==1)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech2,0); // load osd fonts
			   else if(index==2)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech3,0); // load osd fonts
			   else if(index==3)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech4,0); // load osd fonts
			   else if(index==4)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech5,0); // load osd fonts
			   else if(index==5)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech6,0); // load osd fonts
			   else if(index==6)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_All7,0); // load osd fonts
			   else if(index==7)
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech8,0); // load osd fonts
			   else
						mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech9,0); // load osd fonts
			   break;
		   break;
#elif LANGUAGE_TYPE== All14Language_AddJap
		   
	   		case  LANG_Portugues:
				LoadMainTitleFont_1(index);
				break;
			case  LANG_SChina:
				LoadMainTitleFont_1(index);
				break;
			case  LANG_TChina:
				LoadMainTitleFont_1(index);
				break;
			case  LANG_Korea:
				LoadMainTitleFont_1(index);
				break;
			case  LANG_Japan:
				LoadMainTitleFont_1(index);
				break;

		   case  LANG_German:
				   if(index==0)
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German1,0); // load osd fonts
				   else if(index==1)
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German2,0); // load osd fonts
				   else if(index==2)
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German3,0); // load osd fonts
				   else if(index==3)
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German4,0); // load osd fonts
				   else if(index==4)
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German5,0); // load osd fonts
				   else if(index==5)
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German6,0); // load osd fonts
				   else if(index==6)
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_All7,0); // load osd fonts
				   else if(index==7)
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German8,0); // load osd fonts
				   else
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German9,0); // load osd fonts
				   break;
			   break;
		   
			   case  LANG_Italian:
				   if(index==0)
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian1,0); // load osd fonts
				   else if(index==1)
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian2,0); // load osd fonts
				   else if(index==2)
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian3,0); // load osd fonts
				   else if(index==3)
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian4,0); // load osd fonts
				   else if(index==4)
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian5,0); // load osd fonts
				   else if(index==5)
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian6,0); // load osd fonts
				   else if(index==6)
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_All7,0); // load osd fonts
				   else if(index==7)
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian8,0); // load osd fonts
				   else
							mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian9,0); // load osd fonts
				   break;
				  break;
				  
				  case	LANG_Swedish:
						  if(index==0)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish1,0); // load osd fonts
						  else if(index==1)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish2,0); // load osd fonts
						  else if(index==2)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish3,0); // load osd fonts
						  else if(index==3)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish4,0); // load osd fonts
						  else if(index==4)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish5,0); // load osd fonts
						  else if(index==5)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish6,0); // load osd fonts
						  else if(index==6)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_All7,0); // load osd fonts
						  else if(index==7)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish8,0); // load osd fonts
						  else
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish9,0); // load osd fonts
						  break;
					  break;
				  
					  case	LANG_Polish:
						  if(index==0)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish1,0); // load osd fonts
						  else if(index==1)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish2,0); // load osd fonts
						  else if(index==2)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish3,0); // load osd fonts
						  else if(index==3)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish4,0); // load osd fonts
						  else if(index==4)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish5,0); // load osd fonts
						  else if(index==5)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish6,0); // load osd fonts
						  else if(index==6)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish7,0); // load osd fonts
						  else if(index==7)
			#if CenterTitleText
								  mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_English8,0); // load osd fonts
			#else
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish8,0); // load osd fonts
			#endif
						  else
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish9,0); // load osd fonts
						  break;
					  break;
				  
					  case	LANG_Czech:
						  if(index==0)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech1,0); // load osd fonts
						  else if(index==1)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech2,0); // load osd fonts
						  else if(index==2)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech3,0); // load osd fonts
						  else if(index==3)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech4,0); // load osd fonts
						  else if(index==4)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech5,0); // load osd fonts
						  else if(index==5)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech6,0); // load osd fonts
						  else if(index==6)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_All7,0); // load osd fonts
						  else if(index==7)
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech8,0); // load osd fonts
						  else
								   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech9,0); // load osd fonts
						  break;
					  break;

#else



	case  LANG_German:
		if(index==0)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German1,0); // load osd fonts
		else if(index==1)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German2,0); // load osd fonts
		else if(index==2)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German3,0); // load osd fonts
		else if(index==3)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German4,0); // load osd fonts
		else if(index==4)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German5,0); // load osd fonts
		else if(index==5)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German6,0); // load osd fonts
		else if(index==6)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_All7,0); // load osd fonts
		else if(index==7)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German8,0); // load osd fonts
		else
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_German9,0); // load osd fonts
		break;
	break;

	case  LANG_Italian:
		if(index==0)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian1,0); // load osd fonts
		else if(index==1)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian2,0); // load osd fonts
		else if(index==2)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian3,0); // load osd fonts
		else if(index==3)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian4,0); // load osd fonts
		else if(index==4)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian5,0); // load osd fonts
		else if(index==5)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian6,0); // load osd fonts
		else if(index==6)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_All7,0); // load osd fonts
		else if(index==7)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian8,0); // load osd fonts
		else
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Italian9,0); // load osd fonts
		break;
       break;

#if LANGUAGE_TYPE == EUROPE

	case  LANG_Swedish:
		if(index==0)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish1,0); // load osd fonts
		else if(index==1)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish2,0); // load osd fonts
		else if(index==2)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish3,0); // load osd fonts
		else if(index==3)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish4,0); // load osd fonts
		else if(index==4)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish5,0); // load osd fonts
		else if(index==5)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish6,0); // load osd fonts
		else if(index==6)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_All7,0); // load osd fonts
		else if(index==7)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish8,0); // load osd fonts
		else
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Swedish9,0); // load osd fonts
		break;
	break;

	case  LANG_Polish:
		if(index==0)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish1,0); // load osd fonts
		else if(index==1)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish2,0); // load osd fonts
		else if(index==2)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish3,0); // load osd fonts
		else if(index==3)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish4,0); // load osd fonts
		else if(index==4)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish5,0); // load osd fonts
		else if(index==5)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish6,0); // load osd fonts
		else if(index==6)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish7,0); // load osd fonts
		else if(index==7)
			#if CenterTitleText
	     		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_English8,0); // load osd fonts
			#else
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish8,0); // load osd fonts
			#endif
		else
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Polish9,0); // load osd fonts
		break;
	break;

	case  LANG_Czech:
		if(index==0)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech1,0); // load osd fonts
		else if(index==1)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech2,0); // load osd fonts
		else if(index==2)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech3,0); // load osd fonts
		else if(index==3)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech4,0); // load osd fonts
		else if(index==4)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech5,0); // load osd fonts
		else if(index==5)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech6,0); // load osd fonts
		else if(index==6)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_All7,0); // load osd fonts
		else if(index==7)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech8,0); // load osd fonts
		else
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Czech9,0); // load osd fonts
		break;
	break;
#else

	case  LANG_Portugues:
		if(index==0)
      			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues1,0); // load osd fonts
		else if(index==1)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues2,0); // load osd fonts
		else if(index==2)
      			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues3,0); // load osd fonts
		else if(index==3)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues4,0); // load osd fonts
		else if(index==4)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues5,0); // load osd fonts
		else if(index==5)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues6,0); // load osd fonts
		else if(index==6)
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_All7,0); // load osd fonts
		else if(index==7)
    			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues8,0); // load osd fonts
		else
     			 mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Portugues9,0); // load osd fonts
		break;
#if CenterTitleText
	case  LANG_SChina:
		if(index==0)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina1,0); // load osd fonts	
		else if(index==1)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina2,0); // load osd fonts	
		else if(index==2)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina3,0); // load osd fonts	
		else if(index==3)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina4,0); // load osd fonts	
		else if(index==4)
         	mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina5,0); // load osd fonts	
		else if(index==5)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina6,0); // load osd fonts	
		else if(index==6)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina7,0); // load osd fonts	
		else if(index==7)
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina8,0); // load osd fonts	
		else
      		mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_SChina9,0); // load osd fonts	
		break;

#else
	case  LANG_SChina:
		if(index==0)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina1, sizeof(tMainFont_SChina1));
		else if(index==1)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina2, sizeof(tMainFont_SChina2));
		else if(index==2)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina3, sizeof(tMainFont_SChina3));
		else if(index==3)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina4, sizeof(tMainFont_SChina4));
		else if(index==4)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina5, sizeof(tMainFont_SChina5));
		else if(index==5)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina6, sizeof(tMainFont_SChina6));
		else if(index==6)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina7, sizeof(tMainFont_SChina7));
		else if(index==7)
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina8, sizeof(tMainFont_SChina8));
		else
            Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_SChina9, sizeof(tMainFont_SChina9));
		break;

#endif
#endif

#endif
 }


}
