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
#include "MainCompFont_1.h"
#include "debug.h"

#if LANGUAGE_TYPE== All14Language_AddJap
void LoadMainTitleFont_1(BYTE index)
{
	switch(UserPrefLanguage)
 	{
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

		case  LANG_Japan:
			if(index==0)
			   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Japan1,0); // load osd fonts  
			else if(index==1)
			   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Japan2,0); // load osd fonts  
			else if(index==2)
			   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Japan3,0); // load osd fonts  
			else if(index==3)
			   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Japan4,0); // load osd fonts  
			else if(index==4)
			   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Japan5,0); // load osd fonts  
			else if(index==5)
			   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Japan6,0); // load osd fonts  
			else if(index==6)
			   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Japan7,0); // load osd fonts  
			else if(index==7)
			   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Japan8,0); // load osd fonts  
			else
			   mStar_LoadCompressedFont(MAIN_MENU_FONT_START, &tMainFont_Japan9,0); // load osd fonts  
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
		case  LANG_Japan:
			if(index==0)
				Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Japan1, sizeof(tMainFont_Japan1));
			else if(index==1)
				Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Japan2, sizeof(tMainFont_Japan2));
			else if(index==2)
				Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Japan3, sizeof(tMainFont_Japan3));
			else if(index==3)
				Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Japan4, sizeof(tMainFont_Japan4));
			else if(index==4)
				Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Japan5, sizeof(tMainFont_Japan5));
			else if(index==5)
				Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Japan6, sizeof(tMainFont_Japan6));
			else if(index==6)
				Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Japan7, sizeof(tMainFont_Japan7));
			else if(index==7)
				Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Japan8, sizeof(tMainFont_Japan8));
			else
				Osd_LoadFont(MAIN_MENU_FONT_START, &tMainFont_Japan9, sizeof(tMainFont_Japan9));
			break;
		
		#endif
	}
}
#endif
