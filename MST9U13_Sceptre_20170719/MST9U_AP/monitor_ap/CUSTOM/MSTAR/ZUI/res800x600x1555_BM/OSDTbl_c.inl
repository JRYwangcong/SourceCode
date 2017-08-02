
#include "ZUI_MAIN_MENU_tables_c.inl"
#include "ZUI_MAIN_MENU_alphatable_c.inl"
#include "ZUI_IDLE_tables_c.inl"
#include "ZUI_IDLE_alphatable_c.inl"
#include "ZUI_AUTO_ADJUST_tables_c.inl"
#include "ZUI_AUTO_ADJUST_alphatable_c.inl"
#include "ZUI_DIRMSG_MENU_tables_c.inl"
#include "ZUI_DIRMSG_MENU_alphatable_c.inl"
#include "ZUI_LOGO_tables_c.inl"
#include "ZUI_LOGO_alphatable_c.inl"



OSDPAGETABLE code OsdTables [] =
{
    { 0, NULL,
         //NULL,
         NULL,
         NULL,
         0, 0, 0, 0, 0, TRUE, 0, TRUE,
         CLRKEY_LO_R, CLRKEY_LO_G, CLRKEY_LO_B, CLRKEY_HI_R, CLRKEY_HI_G, CLRKEY_HI_B,
         0 },
    {HWND_MAIN_MENU_MAX, _GUI_WindowList_Zui_Main_Menu,
         //_GUI_WindowsDrawStyleList_Zui_Main_Menu,
         _GUI_WindowsAlphaList_Zui_Main_Menu,
         NULL,
         E_OSD_GWIN0, ZUI_MAIN_MENU_XSTART, ZUI_MAIN_MENU_YSTART, ZUI_MAIN_MENU_WIDTH, ZUI_MAIN_MENU_HEIGHT, 0, 44, 1,
         CLRKEY_LO_R, CLRKEY_LO_G, CLRKEY_LO_B, CLRKEY_HI_R, CLRKEY_HI_G, CLRKEY_HI_B,
         0 },
    {HWND_IDLE_MAX, _GUI_WindowList_Zui_Idle,
         //_GUI_WindowsDrawStyleList_Zui_Idle,
         _GUI_WindowsAlphaList_Zui_Idle,
         NULL,
         E_OSD_GWIN0, ZUI_IDLE_XSTART, ZUI_IDLE_YSTART, ZUI_IDLE_WIDTH, ZUI_IDLE_HEIGHT, 1, 63, 1,
         CLRKEY_LO_R, CLRKEY_LO_G, CLRKEY_LO_B, CLRKEY_HI_R, CLRKEY_HI_G, CLRKEY_HI_B,
         0 },
    {HWND_AUTO_ADJUST_MAX, _GUI_WindowList_Zui_Auto_Adjust,
         //_GUI_WindowsDrawStyleList_Zui_Auto_Adjust,
         _GUI_WindowsAlphaList_Zui_Auto_Adjust,
         NULL,
         E_OSD_GWIN0, ZUI_AUTO_ADJUST_XSTART, ZUI_AUTO_ADJUST_YSTART, ZUI_AUTO_ADJUST_WIDTH, ZUI_AUTO_ADJUST_HEIGHT, 0, 44, 1,
         CLRKEY_LO_R, CLRKEY_LO_G, CLRKEY_LO_B, CLRKEY_HI_R, CLRKEY_HI_G, CLRKEY_HI_B,
         0 },
    {HWND_DIRMSG_MENU_MAX, _GUI_WindowList_Zui_Dirmsg_Menu,
         //_GUI_WindowsDrawStyleList_Zui_Dirmsg_Menu,
         _GUI_WindowsAlphaList_Zui_Dirmsg_Menu,
         NULL,
         E_OSD_GWIN0, ZUI_DIRMSG_MENU_XSTART, ZUI_DIRMSG_MENU_YSTART, ZUI_DIRMSG_MENU_WIDTH, ZUI_DIRMSG_MENU_HEIGHT, 0, 44, 1,
         CLRKEY_LO_R, CLRKEY_LO_G, CLRKEY_LO_B, CLRKEY_HI_R, CLRKEY_HI_G, CLRKEY_HI_B,
         0 },
    {HWND_LOGO_MAX, _GUI_WindowList_Zui_Logo,
         //_GUI_WindowsDrawStyleList_Zui_Logo,
         _GUI_WindowsAlphaList_Zui_Logo,
         NULL,
         E_OSD_GWIN0, ZUI_LOGO_XSTART, ZUI_LOGO_YSTART, ZUI_LOGO_WIDTH, ZUI_LOGO_HEIGHT, 0, 63, 1,
         CLRKEY_LO_R, CLRKEY_LO_G, CLRKEY_LO_B, CLRKEY_HI_R, CLRKEY_HI_G, CLRKEY_HI_B,
         0 },
};

OSDPAGE_TRANSEFF_TABLE code OsdTransEffTables [] =
{
    { 0,
         0 },
    { E_TRANSEFF_IN_NONE,
         E_TRANSEFF_OUT_NONE },
    { E_TRANSEFF_IN_NONE,
         E_TRANSEFF_OUT_NONE },
    { E_TRANSEFF_IN_NONE,
         E_TRANSEFF_OUT_NONE },
    { E_TRANSEFF_IN_NONE,
         E_TRANSEFF_OUT_NONE },
    { E_TRANSEFF_IN_NONE,
         E_TRANSEFF_OUT_NONE },
};
