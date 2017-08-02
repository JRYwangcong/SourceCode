#include "Board.h"
#include "Global.h"
#include "Ms_rwreg.h"
#include "drv3DLUT.h"
#include "COLOR_VERIFY.H"

#if ENABLE_MSTV_UART_DEBUG && COLORVERIFY_DEBUG
code RegUnitType tb_FCC_TABLE[] =
{
    {SC7_80, 0x64},     //target 1 cb, 100,     Red
    {SC7_81, 0xd4}, //target 1 cr, 212,     Red
    {SC7_82, 0x48},     //target 2 cb, 72,  Green
    {SC7_83, 0x3a},     //target 2 cr, 58,  Green
    {SC7_84, 0xd4},     //target 3 cb, 212,     Blue
    {SC7_85, 0x72},     //target 3 cr, 114,     Blue
    {SC7_86, 0x9c}, //target 4 cb, 156,     Cyan
    {SC7_87, 0x2c},     //target 4 cr, 44,  Cyan
    {SC7_88, 0xb8},     //target 5 cb, 184,     Magenta
    {SC7_89, 0xc6},     //target 5 cr, 198,     Magenta
    {SC7_8A, 0x2c},     //target 6 cb, 44,  Yellow
    {SC7_8B, 0x8e},     //target 6 cr, 142,     Yellow
    {SC7_8C, 0x80},     //target 7 cb, 128,     White
    {SC7_8D, 0x80},     //target 7 cr, 128,     White
    {SC7_8E, 0x80},     //target 8 cb, 128,     Black
    {SC7_8F, 0x80}, //target 8 cr, 128,     Black
    {SC7_92, 0xff}, //target 1 boundary, [7:6] cb_u, [5:4] cb_d, [3:2] cr_u, [1:0] cr_d
    {SC7_93, 0xff}, //target 2 boundary, [7:6] cb_u, [5:4] cb_d, [3:2] cr_u, [1:0] cr_d
    {SC7_94, 0xff}, //target 3 boundary, [7:6] cb_u, [5:4] cb_d, [3:2] cr_u, [1:0] cr_d
    {SC7_95, 0xff}, //target 4 boundary, [7:6] cb_u, [5:4] cb_d, [3:2] cr_u, [1:0] cr_d
    {SC7_96, 0xff}, //target 5 boundary, [7:6] cb_u, [5:4] cb_d, [3:2] cr_u, [1:0] cr_d
    {SC7_97, 0xff}, //target 6 boundary, [7:6] cb_u, [5:4] cb_d, [3:2] cr_u, [1:0] cr_d
    {SC7_98, 0xff}, //target 7 boundary, [7:6] cb_u, [5:4] cb_d, [3:2] cr_u, [1:0] cr_d
    {SC7_99, 0xff}, //target 8 boundary, [7:6] cb_u, [5:4] cb_d, [3:2] cr_u, [1:0] cr_d
    {SC7_9A, 0xff}, //target 9 boundary, [5:3] cb_ud, [2:0] cr_ud,

    {SC7_9B, 0xff}, //target 1 gain, [7:4], equal 15/16             //target 2 gain, [3:0], equal 15/16
    {SC7_9C, 0xff}, //target 3 gain, [7:4], equal 15/16         //target 4 gain, [3:0], equal 15/16
    {SC7_9D, 0xff}, //target 5 gain, [7:4], equal 15/16         //target 6 gain, [3:0], equal 15/16
    {SC7_9E, 0xff}, //target 7 gain, [7:4], equal 15/16         //target 8 gain, [3:0], equal 15/16

    {SC7_90,0xff},  //target 0~9 enable, [0] for target 1 and 2, [1:7] for target 2~9

    {SC0_00, 0x00},
};

//>>===========================================

void msFCCLoader(void)
{
    xdata BYTE  ucBank,ucTmp;

    ucBank = _msReadByte(SC0_00);
    _msWriteByteMask(SCB_A0, BIT4|BIT0   , BIT4|BIT0);
    for(ucTmp = 0; ucTmp < sizeof( tb_FCC_TABLE ) / sizeof( RegUnitType ); ucTmp++ )
        _msWriteByte( tb_FCC_TABLE[ucTmp].u16Reg, tb_FCC_TABLE[ucTmp].u8Value );

    _msWriteByte(SC0_00, ucBank);

}
//<<===========================================================
//>>===============msHistrogram_Test=============================
void msHistrogram_Test(BYTE bWindow)
{
    xdata BYTE  ucBank,ucTmp;
    xdata WORD  uwArray32[32];                                  /*use to Story Histrogram32's DLC data*/

    xdata WORD  h_start =   0,  v_start =   0;
    xdata WORD  h_end   = PanelWidth,  v_end   = PanelHeight;
    //xdata DWORD Total_Count,Total_Weight;                   /*use to Story Temp.Histrogram DLC data */
    xdata DWORD Total_Weight;                   /*use to Story Temp.Histrogram DLC data */

    //PanelWidth
    ucBank = _msReadByte(SC0_00);

#if ENABLE_HDMI_INPUT
    if( gScInfo.InputColor == INPUT_RGB )
        _msWriteByteMask(SC7_3B,_BIT6,_BIT6);                    //RGB mode enable
    else
#endif
        _msWriteByteMask(SC7_3B,0x00,_BIT6);                     //RGB mode enable

    _msWriteByteMask(SC7_B8,_BIT4,_BIT4);                    //New Y' histogram option enable. 0: Histogram in Y. 1: Histogram in Y'.
    _msWriteByteMask(SC7_B5,_BIT1,_BIT1);                //Histogram accelerate mode ON
    _msWriteByteMask(SC7_B8, 0, (BIT3|BIT2) );               //clear hand shaking
    if( MAIN_WINDOW == bWindow )
        _msWriteByteMask( SC7_B8, BIT1, BIT1|BIT0);              //Main Histrogram window enable
    else
        _msWriteByteMask( SC7_B8, BIT0, BIT1|BIT0);              //Sub Histrogram window enable

//>>--------------------Range-------------------------------------------------------
    #if 1 //V range doesnt change , but H range is moved to SC7
    _msWriteByte( SC3_B6, v_start/8 );                           // set statistic start range (unit: 8 vertical lines)
    _msWriteByte( SC3_B7, v_end  /8 );                            // set statistic end range (uint: 8 vertical lines)
    _msWriteByteMask( SC3_B5, BIT0, BIT0 );                      // set statistic vertical range is valid

    _msWrite2Byte( SC7_24, h_start);             // set statistic start range (unit: 1 pixel)
    _msWrite2Byte( SC7_26, h_end );                          // set statistic start range (unit: 1 pixel)
    _msWriteByteMask( SC7_27, (MAIN_WINDOW==bWindow)?(BIT7):(BIT6), BIT7|BIT6 );                 // 1 for set manual H range setting enable in Main window

    printData("V_Start_(SC3_B6)==[%d]\n\r",_msReadByte(SC3_B6)*8);
    printData("V_End_(SC3_B7)==[%d]\n\r",_msReadByte(SC3_B7)*8);
    printData("H_Start_(SC7_2524)==[%d]\n\r",h_start);
    printData("H_End_(SC7_2726)==[%d]\n\r",h_end);
    #endif
//<<-----------------------------------------------------------------------------------
//>>--------------------Enable Histogram Report----------------
    _msWriteByteMask( SC7_B8, ((MAIN_WINDOW==bWindow)?(BIT1):(BIT0))|BIT2, BIT2|BIT1|BIT0 );                 // 1 for set manual H range setting enable in Main window
//<<-----------------------------------------------------------------------------------
//>>--------------------32 Section Histogram report----------------
    while(!(_msReadByte(SC7_B8) & _BIT3))  /*SC7_B8[3] HS_REQ:Hand Shaking Acknowledge (Read only) */
    {
    }
    printData("111111.(total)==[%x]\n\r",(PanelWidth/64*PanelHeight));
    printData("111111.(Weight)==[%x]\n\r",(PanelWidth/64*PanelHeight*256));
    for (ucTmp = 0; ucTmp < 32; ucTmp++)
    {
        uwArray32[ucTmp] = ((WORD) _msRead2Byte(SC8_C0+ucTmp*2));
        printData("ADD[]==[%x]\n\r",(SC8_C0+ucTmp*2));

        printData("uwArray32[]==[%x]\n\r",uwArray32[ucTmp]);
    }

//<<-----------------------------------------------------------------------------------
//>>--------------------Total Weight and Count , Min. and Maximum report----------------
    //Total_Count=_msRead2Byte(SC7_34); //removed in Russell
    Total_Weight=_msRead2Byte(SC7_36);
    printData("222222.minimum pixel value ==[%x]\n\r",_msReadByte(SC7_39));
    printData("222222.maximum pixel value ==[%x]\n\r",_msReadByte(SC7_3A));

    //printData("222222.Sum of pixel count==[%x]\n\r",Total_Count);
    printData("222222.Sum of total weight.==[%x]\n\r",Total_Weight);
//<<-----------------------------------------------------------------------------------
_msWriteByte(SC0_00, ucBank);

}
//<<===========================================================
void ms2DHPeakingTest(BYTE bScalerWin ,BYTE ucSharpness, BYTE ucStep )
{
#define NO_CHANGE_STEP      0x80
#define NO_CHANGE_SHARPNESS 0xFF


    BYTE xdata uci = 0;

    //Over/Under shoort Setting  //main and sub use the same setting
    for( uci = 0; uci < 8; uci ++ )
    {
         _msWriteByteMask(SCB_40+uci, 0x00,0xFF);        //overshoort：SCB_40~SCB_47
         _msWriteByteMask(SCB_48+uci, 0xFF,0xFF);        //undershoort：SCB_48~SCB_4F
    }
    if(bScalerWin == MAIN_WINDOW)
    {
        //>>----main win H peaking initial------------------
        //Enable
        _msWriteByteMask( SCB_20, BIT0   , BIT0 );   //main post peaking enable
        _msWriteByteMask( SCB_21, 0xFF   , 0xFF );   //main Bank1~8 peaking enable
        //Term Setting
        #if 0
        _msWrite2ByteMask( SCB_38, 0x0123    , 0x7777 ); //main win peaking term1~4 select
        _msWrite2ByteMask( SCB_3A, 0x0123    , 0x7777 ); //main win peaking term5~8 select
        _msWrite2ByteMask( SCB_3C, 0x4567    , 0x7777 ); //main win peaking term9~12 select
        _msWrite2ByteMask( SCB_3E, 0x4567    , 0x7777 ); //main win peaking term13~16 select
        #else
        for( uci = 0; uci < 4; uci ++ )
        {   //Term Setting for main win
             _msWriteByteMask(SCB_38+uci*2   , (uci*2<<4)|(uci*2+1)  ,0x77);
             _msWriteByteMask(SCB_38+uci*2+1, (uci*2<<4)|(uci*2+1) ,0x77);
        }
        #endif

        //Post Coring Setting
        _msWriteByteMask( SCB_26, 0x42   , 0xFF );           //[7:4]:Coring_TH_2 ,[3:0]:Coring_TH_1
        for( uci = 0; uci < 8; uci ++ )
        {
             _msWriteByteMask(SCB_30+uci, 0x20,0x3F);        //main win band 1~8 peaking coef
        }
        //<<--------------------------
        if( NO_CHANGE_SHARPNESS == ucSharpness )
            ucSharpness = _msReadByte(SCB_27)&0x3F;    // MAIN window user sharpness adjust
        else
            ucSharpness&=0x3F;

        _msWriteByte(SCB_27,  ucSharpness);
        printData("111 ucSharpness===%x",_msReadByte(SCB_27));

        if( NO_CHANGE_STEP == ucStep )
            ucStep = (_msReadByte(SCB_22)&0x03);                 // MAIN window peaking step
        else
            ucStep&=0x03;

        printData("111 ucStep===%x",ucStep);
        _msWriteByte(SCB_22,  (ucStep<<6 |ucStep<<4|ucStep<<2|ucStep));  //for band1~4
        _msWriteByte(SCB_23,  (ucStep<<6 |ucStep<<4|ucStep<<2|ucStep));  //for band5~8
        printData("111 ucStep===%x",_msReadByte(SCB_22));
    }
    else
    {

        //>>----sub win H peaking initial------------------
        //Enable
        _msWriteByteMask( SCB_28, BIT0   , BIT0 );   //sub post peaking enable
        _msWriteByteMask( SCB_29, 0xFF   , 0xFF );   //sub Bank1~8 peaking enable
        //Term Setting
        #if 0
        _msWrite2ByteMask( SCB_58, 0x0123    , 0x7777 ); //main win peaking term1~4 select
        _msWrite2ByteMask( SCB_5A, 0x0123    , 0x7777 ); //main win peaking term5~8 select
        _msWrite2ByteMask( SCB_5C, 0x4567    , 0x7777 ); //main win peaking term9~12 select
        _msWrite2ByteMask( SCB_5E, 0x4567    , 0x7777 ); //main win peaking term13~16 select
        #else
        for( uci = 0; uci < 4; uci ++ )
        {
             _msWriteByteMask(SCB_58+uci*2   , (uci*2<<4)|(uci*2+1)  ,0x77);
             _msWriteByteMask(SCB_59+uci*2+1, (uci*2<<4)|(uci*2+1) ,0x77);
        }
        #endif
        //Post Coring Setting
        _msWriteByteMask( SCB_2E, 0x42   , 0xFF );           //[7:4]:Coring_TH_2 ,[3:0]:Coring_TH_1
        for( uci = 0; uci < 8; uci ++ )
        {
             _msWriteByteMask(SCB_50+uci, 0x20,0x3F);        //sub win band 1~8 peaking coef
        }

        //<<--------------------------
        if( NO_CHANGE_SHARPNESS == ucSharpness )
            ucSharpness = _msReadByte(SCB_2F)&0x3F;    // MWE window user sharpness adjust
        else
            ucSharpness&=0x3F;
        _msWriteByte(SCB_2F,  ucSharpness);

        if( NO_CHANGE_STEP == ucStep )
            ucStep = (_msReadByte(SCB_2A)&0x03);                 // MWE window peaking step
        else
            ucStep&=0x03;

        printData("222 ucStep===%x",ucStep);
        _msWriteByte(SCB_2A,  (ucStep<<6 |ucStep<<4|ucStep<<2|ucStep));  //for band1~4
        _msWriteByte(SCB_2B,  (ucStep<<6 |ucStep<<4|ucStep<<2|ucStep));  //for band5~8
        printData("222 SCB_2B===%x",_msReadByte(SCB_2B));
    }
    //_msWriteByte(SC0_00, ucBank);
}

void msNoiseMasking(BYTE bScalerWin ,BYTE u8Strength, BYTE u8THRD )
{
	//Command1(BIT0:Main/Sub win, BIT1:NM enable, BIT2:LowY enable, BIT3:STD enable)
	//Command2: NM gain coef
	//Command3:THRD
	//EX:normal mode: 49 03 3f ff (main win,NM enable gain=0x3F)
	//EX:Low Y mode:  49 07 3f 20 (main win,NM enable ,LowY mode enable, gain=0x3F/2 and LowY's gain=0x3F,THRD=0x20)

	//1.Normal mode
	if(_bit0_(bScalerWin))
	{
		printData("111.Main window (bScalerWin&BIT0)==>%x",(bScalerWin&BIT0));

		//H direction Noise masking
		_msWriteByteMask(SCB_C0, (_bit1_(bScalerWin)?(BIT7):(0x00))		,BIT7);  	// Y noise-masking enable
		_msWriteByteMask(SCB_C0, (_bit2_(bScalerWin)?((u8Strength<=0x3F? u8Strength/2:0x3F/2)):((u8Strength<=0x3F? u8Strength:0x3F))) 	,0x3F);  	//BIT5~BIT0:4.2 format
		_msWriteByteMask(SCB_C2, 0xFF 	,0xFF); 									//Max/Min value threshold
		_msWriteByteMask(SCB_C1, (_bit1_(bScalerWin)?(BIT7):(0x00))		,BIT7);  	//C noise-masking enable
		_msWriteByteMask(SCB_C1, (_bit2_(bScalerWin)?((u8Strength<=0x3F? u8Strength/2:0x3F/2)):((u8Strength<=0x3F? u8Strength:0x3F))) 	,0x3F);  	//BIT5~BIT0:4.2 format
		_msWriteByteMask(SCB_C3, 0xFF 	,0xFF); 									//Max/Min value threshold
		//V direction Noise masking
		_msWriteByteMask(SCB_24, (_bit1_(bScalerWin)?(BIT7):(0x00))			,BIT7);  	// Y noise-masking enable
		_msWriteByteMask(SCB_24, (_bit2_(bScalerWin)?((u8Strength<=0x3F? u8Strength/2:0x3F/2)):((u8Strength<=0x3F? u8Strength:0x3F))) 	,0x3F);  	//BIT5~BIT0:4.2 format
		_msWriteByteMask(SCB_72, 0xFF 	,0xFF); 									//Max/Min value threshold
		if (_bit1_(bScalerWin))
			printMsg("111.Enable NM");
		else
			printMsg("111. disable NM");
		//2.LOW Y mode
		{
			//H direction:
 			//Y
			{
				if (_bit2_(bScalerWin))
					printMsg("111. Enable Low Y mode");
				else
					printMsg("111. disable Low Y mode");

				_msWriteByteMask(SCB_E0, (_bit2_(bScalerWin)?(BIT0):(0x00))	,BIT0); 		//reg_main_nm_low_y_en
				_msWriteByteMask(SCB_E2, u8THRD	,0xFF);								//reg_main_nm_low_y_th
				_msWriteByteMask(SCB_E4, (u8Strength<=0x3F? u8Strength:0x3F),0x3F);		//reg_main_nm_low_y_gain
				_msWriteByteMask(SCB_E5, (0x00),BIT1|BIT0); 								//reg_main_nm_low_y_step
				printData("111.SCB_E0==%x",_msReadByte(SCB_E0));
				//C
				_msWriteByteMask(SCB_EA, (_bit2_(bScalerWin)?(BIT0):(0x00))	,BIT0); 		//reg_main_nm_low_y_en
				_msWriteByteMask(SCB_EC, u8THRD	,0xFF);								//reg_main_nm_low_y_th
				_msWriteByteMask(SCB_EE, (u8Strength<=0x3F? u8Strength:0x3F),0x3F);		//reg_main_nm_low_y_gain
				_msWriteByteMask(SCB_EF, (0x00)	,BIT1|BIT0); 								//reg_main_nm_low_y_step
				printData("111.SCB_EA==%x",_msReadByte(SCB_EA));
				//V direction:
				_msWriteByteMask(SCB_F4,(_bit2_(bScalerWin)?(BIT0):(0x00))		,BIT0); 		//reg_main_v_nm_low_y_en
				_msWriteByteMask(SCB_F6, u8THRD	,0xFF);								//reg_main_v_nm_low_y_th
				_msWriteByteMask(SCB_E8, (u8Strength<=0x3F? u8Strength:0x3F),0x3F);		//reg_main_v_nm_low_y_gain
				_msWriteByteMask(SCB_E9, (0x00),BIT1|BIT0); 								//reg_main_v_nm_low_y_step
				printData("111.SCB_F4==%x",_msReadByte(SCB_F4));
			}
		}
		//3.STD mode
		{
			if (_bit3_(bScalerWin))
				printMsg("111. Enable STD mode");
			else
				printMsg("111. Disable STD mode");
			//H direction
			_msWriteByteMask(SCB_E0, (_bit3_(bScalerWin)?(BIT1):(0x00))	,BIT1); 	//reg_main_nm_low_y_en
			_msWriteByteMask(SCB_E1, (0x00),BIT1|BIT0); 								//reg_main_nm_std_step
			_msWriteByteMask(SCB_E3, u8THRD,0xFF); 								//reg_main_nm_std_th
			//V direction
			_msWriteByteMask(SCB_F4, (_bit3_(bScalerWin)?(BIT1):(0x00))	,BIT1); 		//reg_main_v_nm_std_en
			_msWriteByteMask(SCB_F5, (0x00),BIT1|BIT0); 								//reg_main_v_nm_std_step
		}
	}
	else
	{

		printMsg("222.Sub window");
		//H direction Noise masking
		_msWriteByteMask(SCB_C8, (_bit1_(bScalerWin)?(BIT7):(0x00))		,BIT7);  	// Y noise-masking enable
		_msWriteByteMask(SCB_C8, (_bit2_(bScalerWin)?((u8Strength<=0x3F? u8Strength/2:0x3F/2)):((u8Strength<=0x3F? u8Strength:0x3F))) 	,0x3F);  	//BIT5~BIT0:4.2 format
		_msWriteByteMask(SCB_CA, 0xFF 	,0xFF); 			//Max/Min value threshold
		_msWriteByteMask(SCB_C9, (_bit1_(bScalerWin)?(BIT7):(0x00))		,BIT7);  	//C noise-masking enable
		_msWriteByteMask(SCB_C9, (_bit2_(bScalerWin)?((u8Strength<=0x3F? u8Strength/2:0x3F/2)):((u8Strength<=0x3F? u8Strength:0x3F))) 	,0x3F);  	//BIT5~BIT0:4.2 format
		_msWriteByteMask(SCB_CB, 0xFF 	,0xFF); 			//Max/Min value threshold
		//V direction Noise masking
		_msWriteByteMask(SCB_2C, (_bit1_(bScalerWin)?(BIT7):(0x00))		,BIT7);  	// Y noise-masking enable
		_msWriteByteMask(SCB_2C, (_bit2_(bScalerWin)?((u8Strength<=0x3F? u8Strength/2:0x3F/2)):((u8Strength<=0x3F? u8Strength:0x3F))) 	,0x3F);  	//BIT5~BIT0:4.2 format
		_msWriteByteMask(SCB_73, 0xFF 	,0xFF); 			//Max/Min value threshold
		if (_bit1_(bScalerWin))
			printMsg("222. Enable NM");
		else
			printMsg("222. Disable NM");

		//2.LOW Y mode
		{
			//Y
			{
				if (_bit2_(bScalerWin))
					printMsg("222. Enable LowY mode");
				else
					printMsg("222. Disable LowY mode");

				_msWriteByteMask(SCB_E0, (_bit2_(bScalerWin)?(BIT4):(0x00))	,BIT4); 		//reg_sub_nm_low_y_en
				_msWriteByteMask(SCB_E6, u8THRD	,0xFF);								//reg_sub_nm_low_y_th
				_msWriteByteMask(SCB_E8, (u8Strength<=0x3F? u8Strength:0x3F),0x3F);		//reg_sub_nm_low_y_gain
				_msWriteByteMask(SCB_E9, (0x00)	,BIT1|BIT0); 								//reg_sub_nm_low_y_step
				printData("2222.SCB_E0==%x",_msReadByte(SCB_E0));
				//C
				_msWriteByteMask(SCB_EA,(_bit2_(bScalerWin)?(BIT4):(0x00))	,BIT4); 		//reg_sub_nm_low_c_en
				_msWriteByteMask(SCB_F0, u8THRD	,0xFF);								//reg_sub_nm_low_c_th
				_msWriteByteMask(SCB_F2, (u8Strength<=0x3F? u8Strength:0x3F),0x3F);		//reg_sub_nm_low_c_gain
				_msWriteByteMask(SCB_F3, (0x00)	,BIT1|BIT0); 								//reg_sub_nm_low_c_step
				printData("2222.SCB_EA==%x",_msReadByte(SCB_EA));
				//V direction:
				_msWriteByteMask(SCB_F4, (_bit2_(bScalerWin)?(BIT4):(0x00))	,BIT4); 		//reg_sub_v_nm_low_y_en
				_msWriteByteMask(SCB_FA, u8THRD	,0xFF);								//reg_sub_v_nm_low_y_th
				_msWriteByteMask(SCB_EC, (u8Strength<=0x3F? u8Strength:0x3F),0x3F);		//reg_sub_v_nm_low_y_gain
				_msWriteByteMask(SCB_ED, (0x00)	,BIT1|BIT0); 								//reg_sub_v_nm_low_y_step
				printData("2222.SCB_F4==%x",_msReadByte(SCB_F4));
			}
		}
		//3.STD mode
		{
			if (_bit3_(bScalerWin))
				printMsg("111. Enable STD mode");
			else
				printMsg("111. Disable STD mode");
			//H direction
			_msWriteByteMask(SCB_E0, (_bit3_(bScalerWin)?(BIT5):(0x00))	,BIT5); 		//reg_sub_nm_low_y_en
			_msWriteByteMask(SCB_E1, (0x00),BIT3|BIT2); 								//reg_sub_nm_std_step
			_msWriteByteMask(SCB_E7, u8THRD,0xFF); 									//reg_main_nm_std_th
			//V direction
			_msWriteByteMask(SCB_F4, (_bit3_(bScalerWin)?(BIT5):(0x00))	,BIT5); 		//reg_sub_v_nm_std_en
			_msWriteByteMask(SCB_F5, (0x00),BIT3|BIT2); 								//reg_sub_v_nm_std_step
		}
	}
}


void msDeMosquito(BYTE u8Enable, BYTE u8Strength ,BYTE u8THRD)
{
/*
	1.開一張有字幕的圖，用來消除字旁邊雜訊
	2.關SPF bypass mode（SC2_70[3]=0)
	3.設定reg_spf_rgb2y_en(SC2_70[7]=1)
	4.開debug mode（SC4_10[7]=1)
	5.開deMosquito（SC4_10[0]=1)
	6.把gain設大（SC4_10[12:8]=0x1F）
	7.設定THRD1（SC4_11[7:0]值愈小愈明顯）
	8.設定THRD2（SC4_12[7:0]值愈大愈明顯）
*/

    _msWriteByteMask(SC2_E0,	0x00		,BIT3|BIT2|BIT1|BIT0);
    _msWriteByteMask(SC2_E0,	(IsColorspaceRGB()? 0x00:BIT7)		,BIT7);

    _msWriteByteMask(SC4_20,	BIT1			,BIT1);  //Alpha LPF enable

    _msWriteByteMask(SC4_20,	u8Enable	,BIT7|BIT0);  //BIT0:enable ,BIT7：debug mode

    _msWriteByteMask(SC4_21,	(u8Strength<=0x1F? u8Strength:0x1F) 	,0x1F);  //BIT12~BIT8
    _msWriteByteMask(SC4_22,	abs(0xFF-u8THRD) 		,0xFF);  //DMS threshold 1
    _msWriteByteMask(SC4_24,	u8THRD 		,0xFF);  //DMS threshold 2

    //_msWriteByteMask(SC4_23, BIT1|BIT0		,BIT1|BIT0);  //reg_lut_step_hor_f2
    //_msWriteByteMask(SC4_25, BIT2|BIT1|BIT0		,BIT2|BIT1|BIT0);  //reg_lut_step_cen_f2

}

void msSpikeNR(BYTE u8Enable, BYTE u8Strength ,BYTE u8THRD)
{
/*
	開一張有雜點的圖，用來消除圖片上的雜點：
	1.關SPF bypass mode（SC2_70[3]=0)
	2.設定reg_spf_rgb2y_en(SC2_70[7]=0)
	3.關SPK_NR motion mode（SC4_50[3]=0）
	4.開SPK_NR開關（SC4_50[0]=1）
	5.設定SPK_NR強度（SC4_50[11:8]=0x0f)
	6.設定SPK_NR THRD1（SC4_51[12:8]，值愈大效果愈強）
	7.設定SPK_NR THRD0（SC4_51[7:0]，值愈大效果愈強）
	8.設定SPK_NR THRD2（SC4_52[7:0]，值愈小效果愈強）
	9.設定SPK_NR PTH1 Step（SC4_53[6:4]，值愈大效果愈強）
	10.設定SPK_NR PTH0 Step（SC4_53[2:0]，值愈大效果愈強）
	11.設定SPK_NR PTH2 Step（SC4_53[10:8]，值愈大效果愈強）
*/

    _msWriteByteMask(SC2_E0,	0x00		,BIT3|BIT2|BIT1|BIT0);
    _msWriteByteMask(SC2_E0,	(IsColorspaceRGB()? 0x00:BIT7)		,BIT7);

    _msWriteByteMask(SC4_A0,	u8Enable&BIT0	,BIT3|BIT0);  //BIT0:enable ,BIT3 motion mode

    _msWriteByteMask(SC4_A1,	(u8Strength<=0x0F? u8Strength:0x0F) 	,0x0F);  //BIT11~BIT8

    _msWriteByteMask(SC4_A2,	u8THRD 		,0xFF);  // threshold 0  BIT7~0
    _msWriteByteMask(SC4_A3,	u8THRD&0x1F 		,0xFF);  // threshold 1  BIT12~8
    _msWriteByteMask(SC4_A4,	abs(0xFF-u8THRD) 		,0xFF);  // threshold 2 BIT7~0

    //_msWriteByteMask(SC4_A6, BIT6|BIT5|BIT4|BIT2|BIT1|BIT0 		,BIT6|BIT5|BIT4|BIT2|BIT1|BIT0);  // STEP11 21 31
    //_msWriteByteMask(SC4_A7, BIT2|BIT1|BIT0	,BIT2|BIT1|BIT0);  // STEP22
}

void msDeBlocking(BYTE u8Enable, /*BYTE u8Strength ,*/BYTE u8THRD)
{
/*
	畫面因為壓縮會有線的雜訊，用來消線的雜訊
	1.關SPF bypass mode（SC2_70[3:0]=0)
	2.設定reg_spf_rgb2y_en(SC2_70[7]=0)

	2.開de blocking 開關：SC2_10[0]=1
	3.開debug mode（SC2_40[7]=1)
	4.關iir mode（SC2_40[6]=0)
	5.開enable (SC2_40[0]=1)
	6.設定THRD（SC2_40[15:8]=0x00)
	7.設定STEP（SC2_40[3:2]=0~3)
*/

    _msWriteByteMask(SC2_E0,	0x00		,BIT3|BIT2|BIT1|BIT0);
    _msWriteByteMask(SC2_E0,	(IsColorspaceRGB()? 0x00:BIT7)		,BIT7);

    _msWriteByteMask(SC2_20,	u8Enable&BIT0	,BIT0);  //BIT0:enable
    _msWriteByteMask(SC2_80,	u8Enable&(BIT7|BIT6|BIT0)	,BIT7|BIT6|BIT0);  //BIT7:debug mode ,BIT6: iir mode BIT0:blockiness_en_f2
    //_msWriteByteMask(SC2_80,	BIT3|BIT2	,BIT3|BIT2);  //De-blocking coarse detect step F2

    _msWriteByteMask(SC2_81,	u8THRD	,0xFF);  //De-blocking coarse active threshold F2


}
void msSNR(BYTE u8Enable, BYTE u8Strength ,BYTE u8THRD)
{
/*
	指的是APN裡的SNR：一樣可以達到消除雜訊的功能，打糊畫面（打dot pattern試）
	1.關SPF bypass mode（SC2_70[3:0]=0)
	2.設定reg_spf_rgb2y_en(SC2_70[7]=1)

	3.開SNR開關：SC2_30[0]=1
	4.關SNR motion mode：SC2_30[1]=0
	5.設定SNR active THRD：SC2_30[15:8]=0xFF（作用？）
	6.設定SNR 強度：SC2_31[3:0]
	7.設定SNR Alpha Step：SC2_31[7:5]（作用？）
	8.設定SNR LUT：SC2_34/35/36/37為最大值（0xFFFF）
*/
    _msWriteByteMask(SC2_E0,	0x00		,BIT3|BIT2|BIT1|BIT0);
    _msWriteByteMask(SC2_E0,	/*(IsColorspaceRGB()? 0x00:BIT7)*/BIT7		,BIT7);

    _msWriteByteMask(SC2_60,	u8Enable&BIT0		,BIT1|BIT0);  //BIT0: enable   BIT1:motion mode
    _msWriteByteMask(SC2_62,	(u8Strength<=0x0F? u8Strength:0x0F) 	,0x0F);  //BIT3~BIT0
    _msWriteByteMask(SC2_61,	u8THRD	,0xFF);  	//SNR active threshold

    _msWrite2ByteMask( SC2_68, 0xFFFF , 0xFFFF );
    _msWrite2ByteMask( SC2_6A, 0xFFFF , 0xFFFF );
    _msWrite2ByteMask( SC2_6C, 0xFFFF , 0xFFFF );
    _msWrite2ByteMask( SC2_6E, 0xFFFF , 0xFFFF );
}

void msPreNoiseMasking(BYTE u8Enable, BYTE u8Strength ,BYTE u8THRD)
{
/*
	和noise masking一樣的作用，但該function是在scaling前（把畫面打糊）
	例如開64灰階，階與階的分界設定完後會變模糊
	1.關SPF bypass mode（SC2_70[3:0]=0)
	2.設定reg_spf_rgb2y_en(SC2_70[7]=1)

	3.關閉motion ：SC2_50[1]=0
	4.開pre-Y noise masking：SC2_50[0]=1
	5.設定Y NM Gain：SC2_54[5:0]
	6.設定Y NM max THRD：SC2_55[3:0]
	7.設定Y NM min THRD：SC2_55[7:4]

*/
    _msWriteByteMask(SC2_E0,	0x00		,BIT3|BIT2|BIT1|BIT0);
    _msWriteByteMask(SC2_E0,	/*(IsColorspaceRGB()? 0x00:BIT7)*/BIT7		,BIT7);

    _msWriteByteMask(SC2_A0,	u8Enable&BIT0		,BIT1|BIT0);  //BIT0: enable   BIT1:motion mode
    _msWriteByteMask(SC2_A8,	(u8Strength<=0x3F? u8Strength:0x3F) 	,0x3F);  //BIT5~BIT0
    _msWriteByteMask(SC2_AA,	u8THRD	,0xFF);  	//max/min THRD
}

void msHcoring(BYTE u8Enable, BYTE u8Strength)
{
/*
	New function in Jnior
	main
	1.SCB_08[1:0]=11b		   Y Band1,2 enable
	2.SCB_08[6:4]=111b		  Step=MAX
	3.SCB_08[7]=1				   Highpass enable HPF 開關（0：coring，1：sharpness)
	sub
	4.SCB_08[9:8]=11b			Y Band1,2 enable
	5.SCB_08[14:12]=111b	  Step=MAX
	6.SCB_08[15]=1				   Highpass enable
	7.SCB_09~0C=FFFF		Gain table MAX
*/
    _msWriteByteMask(SCB_A0,	(IsColorspaceRGB()? 0x00:BIT4|BIT0)		,BIT4|BIT0);

	//main window
    _msWriteByteMask(SCB_10,	u8Enable&(BIT7|BIT1|BIT0)		,BIT7|BIT1|BIT0); //BIT1&BIT0: y_band1&band2_h_coring_en BIT7:HPF mode
    _msWriteByteMask(SCB_10,	BIT2		,BIT2); //dither
    _msWriteByteMask(SCB_10,	(u8Strength<=0x07? u8Strength:0x07)<<4 	,BIT6|BIT5|BIT4);  //BIT6~BIT4
    _msWrite2ByteMask( SCB_12, 0xFFFF , 0xFFFF );
    _msWrite2ByteMask( SCB_14, 0xFFFF , 0xFFFF );
    _msWrite2ByteMask( SCB_16, 0xFFFF , 0xFFFF );
    _msWrite2ByteMask( SCB_18, 0xFFFF , 0xFFFF );

	//sub window
    _msWriteByteMask(SCB_04,	u8Enable&(BIT7|BIT1|BIT0)		,BIT7|BIT1|BIT0); //reg_main_y_band1&band2_h_coring_en
    _msWriteByteMask(SCB_04,	BIT2		,BIT2); //dither
    _msWriteByteMask(SCB_04,	(u8Strength<=0x07? u8Strength:0x07)<<4 	,BIT6|BIT5|BIT4);  //BIT6~BIT4
    _msWrite2ByteMask( SCB_06, 0xFFFF , 0xFFFF );
    _msWrite2ByteMask( SCB_08, 0xFFFF , 0xFFFF );
    _msWrite2ByteMask( SCB_0A, 0xFFFF , 0xFFFF );
    _msWrite2ByteMask( SCB_0C, 0xFFFF , 0xFFFF );

}

void msShinnyColor(BYTE u8Enable, BYTE u8Strength ,BYTE u8THRD)
{
/*
	SC9_4A[11]：main開關
	SC9_4A[15:12]：main gain
	SC9_4A[10]：main Rounding

	SC9_4B[11]：main開關
	SC9_4B[15:12]：main gain
	SC9_4B[10]：main Rounding

	SC9_4C[14]： 在計算過程中gain可能會過大這個register可以用來卡overflow用的
	SC9_4C[13]： HBC gain mode（選gain的模式）
*/
    _msWriteByteMask(SC9_98,	u8THRD		,0xFF);  //hbc gain1 threshold
    _msWriteByteMask(SC9_99,	BIT6		,BIT6);  //clamp function for HBC gain

	//main window
    _msWriteByteMask(SC9_95,	( u8Enable > 0 ) ? ( 1<<3 ) : ( 0 )		,BIT3);  //enable
    _msWriteByteMask(SC9_95,	(u8Strength<=0x0F? u8Strength:0x0F)<<4 	,BIT7|BIT6|BIT5|BIT4);  //BIT7~BIT4 gain
    _msWriteByteMask(SC9_95,	BIT2		,BIT2);  //Rounding

	//sub window
    _msWriteByteMask(SC9_97,	( u8Enable > 0 ) ? ( 1<<3 ) : ( 0 )		,BIT3);  //enable
    _msWriteByteMask(SC9_97,	(u8Strength<=0x0F? u8Strength:0x0F)<<4 	,BIT7|BIT6|BIT5|BIT4);  //BIT7~BIT4 gain
    _msWriteByteMask(SC9_97,	BIT2		,BIT2);  //Rounding

}


void Peaking_PostSNR(BYTE u8Enable, BYTE u8THRD)
{
	/*   2d Peaking- Post SNR
	// Post-SNR is non motion adaptive . The user can use port-snr to reduce noise in spatial (2D) domain.
	1.Main win：
	   SCB_58[0]：Main window Gaussian SNR enable
	   SCB_58[1]：Main window Gaussian SNR adaptive factor enable
	   SCB_58[2]：Main window Gaussian SNR Mean selection :
						 0: 3x3  1: 5x5 （設1 LPF效果比較強）
	   SCB_58[5:4]：Main window Gaussian SNR LUT step
	   SCB_58[12:8]：Main window Gaussian SNR threshold
	2.Sub win：
	   SCB_59[0]：Main window Gaussian SNR enable
	   SCB_59[1]：Main window Gaussian SNR adaptive factor enable
	   SCB_59[2]：Main window Gaussian SNR Mean selection :
						 0: 3x3  1: 5x5
	   SCB_59[5:4]：Main window Gaussian SNR LUT step
	   SCB_59[12:8]：Main window Gaussian SNR threshold

	3.LUT table：（由小到大）
	   SCB_59~5F：Gaussian SNR Table 0~7
	*/

	//main window
    _msWriteByteMask(SCB_B0,	BIT2|BIT1	,BIT2|BIT1);  		//[2]: SNR Mean selection  [1]:adaptive factor enable
    _msWriteByteMask(SCB_B0,	BIT5|BIT4	,BIT5|BIT4);  		//[2]: SNR LUT step
    _msWriteByteMask(SCB_B0,	( u8Enable > 0 ) ? (BIT0 ) : ( 0 )		,BIT0);  //enable
    _msWriteByteMask(SCB_B1,	u8THRD&0x0F	,0x0F);  			//[3:0]:Gaussian SNR threshold

	//sub window
    _msWriteByteMask(SCB_B2,	BIT2|BIT1	,BIT2|BIT1);  		//[2]: SNR Mean selection  [1]:adaptive factor enable
    _msWriteByteMask(SCB_B2,	BIT5|BIT4	,BIT5|BIT4);  		//[2]: SNR LUT step
    _msWriteByteMask(SCB_B2,	( u8Enable > 0 ) ? (BIT0 ) : ( 0 )		,BIT0);  //enable
    _msWriteByteMask(SCB_B3,	u8THRD&0x0F	,0x0F);  			//[3:0]:Gaussian SNR threshold

	//LUT table
    _msWrite2ByteMask(SCB_B8,	0x0608	,0xFFFF);
    _msWrite2ByteMask(SCB_BA,	0x0204	,0xFFFF);
    _msWrite2ByteMask(SCB_BC,	0x0000	,0xFFFF);
    _msWrite2ByteMask(SCB_BE,	0x0000	,0xFFFF);
}


//<<===========================================================
//>>===========================================================

void msColor_Test_Funct(BYTE tb1, BYTE tb2)
{
    BYTE xdata uncallvalue = tb2;
    switch(tb1)
    {
        case 1:
            msHistrogram_Test(tb2);   // 01 ,01, 0 or 1  0-> main; 1->sub
            break;

        case 7:
            printMsg("msFCCLoader");
            msFCCLoader();
            _msWriteByte(SC7_90, ( tb2 ==0 ) ? ( 0x00 ) : ( 0xFF ) );
            _msWriteByte(SC0_00, 0x00 );
            break;

    #if ENABLE_3DLUT


        case 8:
            {
                extern void mStar_Setup3DLUT( BYTE u8WinIndex, BYTE u8ColorMode, BOOL bSubWin, BOOL bEnable );
                mStar_Setup3DLUT(0, 0, 0,1);
            }
                break;

    #endif
    }
}
#endif

