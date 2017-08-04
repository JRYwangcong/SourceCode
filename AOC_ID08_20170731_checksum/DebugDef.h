#define Write_mStar		0x10
#define Read_mStar		0x11
#define Write_mStarSubWin 0x12
#define Read_mStarSubWin  0x13
#define Write_mStarMIUWin 0x14
#define Read_mStarMIUWin  0x15
#define Write_mStarADC		0x20
#define Read_mStarADC		0x21
#define Write_mStarOSD		0x22
#define Read_mStarOSD		0x23
#define Write_mStarTCON	0x24
#define Read_mStarTCON		0x25
#define Write_NVRam		0x26
#define Read_NVRam		0x27
#define Write_Device		0x28
#define Read_Device		0x29
//=========================for Auto Backlight==========
#define H1low 0x01
#define H1Height 0x02
#define Alfa   0x03
#define Bata   0x04
#define Send_Msg	0x80
#define Send_MsgValue	0x81
// 2006/8/31 7:55PM by Emily for MWE adjust
#define Write_Matrix		0x30
#define Read_Matrix			0x31
#define Write_DLC			0x32
#define Read_DLC			0x33

#define Send_DebugOnly	0x38

#define DebugCommand		(SIORxBuffer[0]&0x3F)
#define CommandLength		((SIORxBuffer[0]>>6) + 1)
#define DebugAddress		SIORxBuffer[1]
#define DebugData		SIORxBuffer[2]
#define DebugDeviceAddr		SIORxBuffer[3]
#define DebugAddressBank	SIORxBuffer[3]
#define DebugACK	0xFA	// ACK for PC-end
