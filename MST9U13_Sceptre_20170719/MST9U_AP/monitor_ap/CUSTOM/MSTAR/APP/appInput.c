
#define _APPWINDOW_C_
#include <string.h>
#include "Global.h"
#include "appWindow.h"
#include "msScalerAP.h"

#define APPINPUT_DEBUG    1
#if ENABLE_MSTV_UART_DEBUG && APPINPUT_DEBUG
#define APPINPUT_PRINT(format, ...)     printf(format, ##__VA_ARGS__)
#else
#define APPINPUT_PRINT(format, ...)
#endif

////////////////////////////////////////////////////////////////////////
#if ((defined(LianHeChuangXin_Project))&&(ModeName==M3CDG))
#define DftSyncValidCount_VGA      10
#define DftSyncLossCount_VGA       20

#define DftSyncValidCount_DVI      30
#define DftSyncLossCount_DVI       110//150

#define DftSyncValidCount_HDMI     60//20
#define DftSyncLossCount_HDMI      280//150

#define DftSyncValidCount_DP       30
#define DftSyncLossCount_DP        280//150 

#else
#define DftSyncValidCount_VGA      10
#define DftSyncLossCount_VGA       20

#define DftSyncValidCount_DVI      20
#define DftSyncLossCount_DVI       110//150

#define DftSyncValidCount_HDMI     50//20
#define DftSyncLossCount_HDMI      200//150

#define DftSyncValidCount_DP       20
#define DftSyncLossCount_DP        200//150 
#endif
/////////////////////////////////////////////////////////////////////////
///////////////////////  Detect sync stable cnt  init  //////////////////////////
/////////////////////////////////////////////////////////////////////////
void APPInput_DetectSyncStableCntInit( void )
{
    BYTE u8InputPort;

    // setting based on cus spec, can be adjust run time for PC/AV mode
    for( u8InputPort=First_Input_Port; u8InputPort < Input_Nums; u8InputPort++ )
    {
        if(TYPE_ANALOG & g_InputPort[u8InputPort].eIPType)
        {
            msAPISrcSetSyncStableCnt(u8InputPort, DftSyncValidCount_VGA, DftSyncLossCount_VGA);
        }
        else if( (TYPE_DVI_SINGLE & g_InputPort[u8InputPort].eIPType)
               ||(TYPE_DVI_DUAL & g_InputPort[u8InputPort].eIPType) )
        {
            msAPISrcSetSyncStableCnt(u8InputPort, DftSyncValidCount_DVI, DftSyncLossCount_DVI);
        }
        else if( (TYPE_HDMI & g_InputPort[u8InputPort].eIPType)
               ||(TYPE_MHL & g_InputPort[u8InputPort].eIPType)
               ||(TYPE_HDMI_2_0 & g_InputPort[u8InputPort].eIPType) )
        {
            msAPISrcSetSyncStableCnt(u8InputPort, DftSyncValidCount_HDMI, DftSyncLossCount_HDMI);
        }
        else // DP and Others.
        {
            msAPISrcSetSyncStableCnt(u8InputPort, DftSyncValidCount_DP, DftSyncLossCount_DP);
        }
    }
}
/////////////////////////////////////////////////////////////////////////
///////////////////////////////  AP init  ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void APPInput_Init( void )
{
    BYTE u8InputPort;
    APPINPUT_PRINT("\n[appInput] APPInput_Init\r\n");


    // setting based on cus spec, can be adjust run time for PC/AV mode
    for( u8InputPort=First_Input_Port; u8InputPort < Input_Nums; u8InputPort++ )
    {
        if(TYPE_ANALOG & g_InputPort[u8InputPort].eIPType)
        {
            msAPISrcSetSyncStableCnt(u8InputPort, DftSyncValidCount_VGA, DftSyncLossCount_VGA);
        }
        else if( (TYPE_DVI_SINGLE & g_InputPort[u8InputPort].eIPType)
               ||(TYPE_DVI_DUAL & g_InputPort[u8InputPort].eIPType) )
        {
            msAPISrcSetSyncStableCnt(u8InputPort, DftSyncValidCount_DVI, DftSyncLossCount_DVI);
        }
        else if( (TYPE_HDMI & g_InputPort[u8InputPort].eIPType)
               ||(TYPE_MHL & g_InputPort[u8InputPort].eIPType)
               ||(TYPE_HDMI_2_0 & g_InputPort[u8InputPort].eIPType) )
        {
            msAPISrcSetSyncStableCnt(u8InputPort, DftSyncValidCount_HDMI, DftSyncLossCount_HDMI);
        }
        else // DP and Others.
        {
            msAPISrcSetSyncStableCnt(u8InputPort, DftSyncValidCount_DP, (DftSyncLossCount_DP+80)); //mqsh for DP AMD RX480/390 DOS undisplay 20170117
        }


}}


