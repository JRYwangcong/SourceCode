


//Input

#define SoM(x)                 (LOBYTE(Source_Select_Addr)+x)
 BYTE code strSouWindow[2][20]=
{
	{	 SoM(0x00), SoM(0x01), SoM(0x02), SoM(0x03), SoM(0x04), SoM(0x05), SoM(0x06), SoM(0x07), 
		 SoM(0x08), SoM(0x09), SoM(0x0A), SoM(0x0B), SoM(0x0C), SoM(0x0D), SoM(0x0E), SoM(0x0F), 
		 SoM(0x10), SoM(0x11), SoM(0x12), 
	 0},
	{	 SoM(0x13), SoM(0x14), SoM(0x15), SoM(0x16), SoM(0x17), SoM(0x18), SoM(0x19), SoM(0x1A), 
		 SoM(0x1B), SoM(0x1C), SoM(0x1D), SoM(0x1E), SoM(0x1F), SoM(0x20), SoM(0x21), SoM(0x22), 
		 SoM(0x23), SoM(0x24), SoM(0x25), 
	 0}
};

#define DPM(x)                 (LOBYTE(DP_Addr)+x)
 BYTE code strDP_Window[2][6]=
{
	{	 DPM(0x00), DPM(0x01), DPM(0x02), DPM(0x03), DPM(0x04), 
	 0},
	{	 DPM(0x05), DPM(0x06), DPM(0x07), DPM(0x08), DPM(0x09), 
	 0}
};
#if ENABLE_HDMI2ND_INPUT
#define HDM2(x)                 (LOBYTE(HDMI2_Addr)+x)
 BYTE code strHDM2Window[2][11]=
{
	{	 HDM2(0x00), HDM2(0x01), HDM2(0x02), HDM2(0x03), HDM2(0x04), HDM2(0x05), HDM2(0x06), HDM2(0x07), 
		 HDM2(0x08), HDM2(0x09), 
	 0},
	{	 HDM2(0x0A), HDM2(0x0B), HDM2(0x0C), HDM2(0x0D), HDM2(0x0E), HDM2(0x0F), HDM2(0x10), HDM2(0x11), 
		 HDM2(0x12), HDM2(0x13), 
	 0}
};
#define HDM(x)                 (LOBYTE(HDMI_Addr)+x)
 BYTE code strHDM1Window[2][11]=
{
	{	 HDM(0x00), HDM(0x01), HDM(0x02), HDM(0x03), HDM(0x04), HDM(0x05), HDM(0x06), HDM(0x07), 
		 HDM(0x08), HDM(0x09), 
	 0},
	{	 HDM(0x0A), HDM(0x0B), HDM(0x0C), HDM(0x0D), HDM(0x0E), HDM(0x0F), HDM(0x10), HDM(0x11), 
		 HDM(0x12), HDM(0x13), 
	 0}
};
#else

#define HDM(x)                 (LOBYTE(HDMI_Addr)+x)
 BYTE code strHDMWindow[2][10]=
{
	{	 HDM(0x00), HDM(0x01), HDM(0x02), HDM(0x03), HDM(0x04), HDM(0x05), HDM(0x06), HDM(0x07), 
		 HDM(0x08), 
	 0},
	{	 HDM(0x09), HDM(0x0A), HDM(0x0B), HDM(0x0C), HDM(0x0D), HDM(0x0E), HDM(0x0F), HDM(0x10), 
		 HDM(0x08), 
	 0}
};
#endif

#define DVM(x)                 (LOBYTE(DVI_Addr)+x)
 BYTE code strDVIWindow[2][8]=
{
	{	 DVM(0x00), DVM(0x01), DVM(0x02), DVM(0x03), DVM(0x04), DVM(0x05), DVM(0x00), 
	 0},
	{	 DVM(0x00), DVM(0x06), DVM(0x07), DVM(0x08), DVM(0x09), DVM(0x0A), DVM(0x00), 
	 0}
};

//==========================================================================English
#define FPM(x)                 (LOBYTE(MS_Font_Start)+x)
 BYTE code strFPSWindow[2][8]=
{
	{	 FPM(0x00), FPM(0x01), FPM(0x02), FPM(0x03), FPM(0x04), FPM(0x05), FPM(0x00), 
	 0},
	{	 FPM(0x00), FPM(0x06), FPM(0x07), FPM(0x08), FPM(0x09), FPM(0x0A), FPM(0x00), 
	 0}
};

#define MoM(x)                 (LOBYTE(MS_Font_Start)+x)
 BYTE code strMovWindow[2][10]=
{
	{	 MoM(0x00), MoM(0x01), MoM(0x02), MoM(0x03), MoM(0x04), MoM(0x05), MoM(0x06), MoM(0x07), 
		 MoM(0x08), 
	 0},
	{	 MoM(0x09), MoM(0x0A), MoM(0x0B), MoM(0x0C), MoM(0x0D), MoM(0x0E), MoM(0x0F), MoM(0x10), 
		 MoM(0x11), 
	 0}
};

#define RTM(x)                 (LOBYTE(MS_Font_Start)+x)
  BYTE code strRTSWindow[2][8]=
{
	{	 RTM(0x00), RTM(0x01), RTM(0x02), RTM(0x03), RTM(0x04), RTM(0x05), RTM(0x00), 
	 0},
	{	 RTM(0x00), RTM(0x06), RTM(0x07), RTM(0x08), RTM(0x09), RTM(0x0A), RTM(0x00), 
	 0}
};


#define StM(x)                 (LOBYTE(MS_Font_Start)+x)
 BYTE code strStaWindow[2][14]=
{
	{	 StM(0x00), StM(0x01), StM(0x02), StM(0x03), StM(0x04), StM(0x05), StM(0x06), StM(0x07), 
		 StM(0x06), StM(0x08), StM(0x09), StM(0x0A), StM(0x0B), 
	 0},
	{	 StM(0x0C), StM(0x0D), StM(0x0E), StM(0x0F), StM(0x10), StM(0x11), StM(0x12), StM(0x13), 
		 StM(0x12), StM(0x13), StM(0x14), StM(0x15), StM(0x16), 
	 0}
};

#define USM(x)                 (LOBYTE(MS_Font_Start)+x)
 BYTE code strUSEWindow[2][8]=
{
	{	 USM(0x00), USM(0x01), USM(0x02), USM(0x03), USM(0x04), USM(0x05), USM(0x06), 
	 0},
	{	 USM(0x07), USM(0x08), USM(0x09), USM(0x0A), USM(0x0B), USM(0x0C), USM(0x0D), 
	 0}
};

#define ECM(x)                 (LOBYTE(MS_Font_Start)+x)
 BYTE code strECOWindow[2][8]=
{
	{	 ECM(0x00), ECM(0x01), ECM(0x02), ECM(0x03), ECM(0x04), ECM(0x05), ECM(0x06), 
	 0},
	{	 ECM(0x07), ECM(0x08), ECM(0x09), ECM(0x0A), ECM(0x0B), ECM(0x0C), ECM(0x0D), 
	 0}
};

//==========================================================================French

#define StM_Fren(x)                 (LOBYTE(MS_Font_Start)+x)
 BYTE code strStaWindow_Fren[2][15]=
{
	{	 StM_Fren(0x00), StM_Fren(0x01), StM_Fren(0x02), StM_Fren(0x03), StM_Fren(0x04), StM_Fren(0x05), StM_Fren(0x06), StM_Fren(0x07), 
		 StM_Fren(0x08), StM_Fren(0x09), StM_Fren(0x0A), StM_Fren(0x0B), StM_Fren(0x0C), StM_Fren(0x0D), 
	 0},
	{	 StM_Fren(0x0E), StM_Fren(0x0F), StM_Fren(0x10), StM_Fren(0x03), StM_Fren(0x11), StM_Fren(0x12), StM_Fren(0x13), StM_Fren(0x14), 
		 StM_Fren(0x11), StM_Fren(0x15), StM_Fren(0x15), StM_Fren(0x15), StM_Fren(0x16), StM_Fren(0x17), 
	 0}
};


#define MoM_Fren(x)                 (LOBYTE(MS_Font_Start)+x)
 BYTE code strMovWindow_Fren[2][7]=
{
	{	 MoM_Fren(0x00), MoM_Fren(0x01), MoM_Fren(0x02), MoM_Fren(0x03), MoM_Fren(0x04), MoM_Fren(0x05), 
	 0},
	{	 MoM_Fren(0x06), MoM_Fren(0x07), MoM_Fren(0x08), MoM_Fren(0x09), MoM_Fren(0x09), MoM_Fren(0x0A), 
	 0}
};

#define UsM_Fren(x)                 (LOBYTE(MS_Font_Start)+x)
 BYTE code strUseWindow_Fren[2][15]=
{
	{	 UsM_Fren(0x00), UsM_Fren(0x01), UsM_Fren(0x02), UsM_Fren(0x03), UsM_Fren(0x04), UsM_Fren(0x05), UsM_Fren(0x06), UsM_Fren(0x07), 
		 UsM_Fren(0x08), UsM_Fren(0x09), UsM_Fren(0x0A), UsM_Fren(0x0B), UsM_Fren(0x0B), UsM_Fren(0x0C), 
	 0},
	{	 UsM_Fren(0x0D), UsM_Fren(0x0E), UsM_Fren(0x0F), UsM_Fren(0x10), UsM_Fren(0x11), UsM_Fren(0x12), UsM_Fren(0x13), UsM_Fren(0x14), 
		 UsM_Fren(0x15), UsM_Fren(0x16), UsM_Fren(0x17), UsM_Fren(0x18), UsM_Fren(0x19), UsM_Fren(0x1A), 
	 0}
};


//==========================================================================Span
#define StM_Span(x)                 (LOBYTE(MS_Font_Start)+x)
 BYTE code strStaWindow_Span[2][14]=
{
	{	 StM_Span(0x00), StM_Span(0x01), StM_Span(0x02), StM_Span(0x03), StM_Span(0x04), StM_Span(0x05), StM_Span(0x06), StM_Span(0x07), 
		 StM_Span(0x08), StM_Span(0x09), StM_Span(0x08), StM_Span(0x0A), StM_Span(0x0B), 
	 0},
	{	 StM_Span(0x0C), StM_Span(0x0D), StM_Span(0x0E), StM_Span(0x0F), StM_Span(0x10), StM_Span(0x11), StM_Span(0x12), StM_Span(0x13), 
		 StM_Span(0x14), StM_Span(0x15), StM_Span(0x14), StM_Span(0x16), StM_Span(0x17), 
	 0}
};

#define MoM_Span(x)                 (LOBYTE(MS_Font_Start)+x)
 BYTE code strMovWindow_Span[2][13]=
{
	{	 MoM_Span(0x00), MoM_Span(0x01), MoM_Span(0x02), MoM_Span(0x03), MoM_Span(0x04), MoM_Span(0x05), MoM_Span(0x06), MoM_Span(0x07), 
		 MoM_Span(0x07), MoM_Span(0x08), MoM_Span(0x09), MoM_Span(0x0A), 
	 0},
	{	 MoM_Span(0x0B), MoM_Span(0x0C), MoM_Span(0x0D), MoM_Span(0x0E), MoM_Span(0x0F), MoM_Span(0x10), MoM_Span(0x11), MoM_Span(0x12), 
		 MoM_Span(0x13), MoM_Span(0x14), MoM_Span(0x15), MoM_Span(0x16), 
	 0}
};


#define UsM_Span(x)                 (LOBYTE(MS_Font_Start)+x)
 BYTE code strUseWindow_Span[2][13]=
{
	{	 UsM_Span(0x00), UsM_Span(0x01), UsM_Span(0x02), UsM_Span(0x03), UsM_Span(0x04), UsM_Span(0x05), UsM_Span(0x06), UsM_Span(0x07), 
		 UsM_Span(0x08), UsM_Span(0x09), UsM_Span(0x0A), UsM_Span(0x0B), 
	 0},
	{	 UsM_Span(0x0C), UsM_Span(0x0D), UsM_Span(0x0E), UsM_Span(0x0F), UsM_Span(0x10), UsM_Span(0x11), UsM_Span(0x12), UsM_Span(0x13), 
		 UsM_Span(0x14), UsM_Span(0x15), UsM_Span(0x16), UsM_Span(0x17), 
	 0}
};

//==========================================================================TChinese
#define StM_TCH(x)                 (LOBYTE(MS_Font_Start)+x)
 BYTE code strStaWindow_TCH[2][7]=
{
	{	 StM_TCH(0x00), StM_TCH(0x01), StM_TCH(0x02), StM_TCH(0x03), StM_TCH(0x04), StM_TCH(0x05), 
	 0},
	{	 StM_TCH(0x06), StM_TCH(0x07), StM_TCH(0x08), StM_TCH(0x09), StM_TCH(0x0A), StM_TCH(0x0B), 
	 0}
};


#define MoM_TCH(x)                 (LOBYTE(MS_Font_Start)+x)
 BYTE code strMovWindow_TCH[2][7]=
{
	{	 MoM_TCH(0x00), MoM_TCH(0x01), MoM_TCH(0x02), MoM_TCH(0x03), MoM_TCH(0x04), MoM_TCH(0x05), 
	 0},
	{	 MoM_TCH(0x06), MoM_TCH(0x07), MoM_TCH(0x08), MoM_TCH(0x09), MoM_TCH(0x0A), MoM_TCH(0x0B), 
	 0}
};

#define UsM_TCH(x)                 (LOBYTE(MS_Font_Start)+x)
 BYTE code strUseWindow_TCH[2][13]=
{
	{	 UsM_TCH(0x00), UsM_TCH(0x01), UsM_TCH(0x02), UsM_TCH(0x03), UsM_TCH(0x04), UsM_TCH(0x05), UsM_TCH(0x06), UsM_TCH(0x07), 
		 UsM_TCH(0x08), UsM_TCH(0x09), UsM_TCH(0x0A), UsM_TCH(0x0B), 
	 0},
	{	 UsM_TCH(0x0C), UsM_TCH(0x0D), UsM_TCH(0x0E), UsM_TCH(0x0F), UsM_TCH(0x10), UsM_TCH(0x11), UsM_TCH(0x12), UsM_TCH(0x13), 
		 UsM_TCH(0x14), UsM_TCH(0x15), UsM_TCH(0x16), UsM_TCH(0x17), 
	 0}
};


//==========================================================================

#define MSM(x)                 (LOBYTE(MS_Font_Start+0x40)+x)
 BYTE code strMSGWindow[2][15]=
{
	{	 MSM(0x00), MSM(0x01), MSM(0x02), MSM(0x03), MSM(0x04), MSM(0x05), MSM(0x06), MSM(0x07), 
		 MSM(0x08), MSM(0x09), MSM(0x0A), MSM(0x0B), MSM(0x0C), MSM(0x05), 
	 0},
	{	 MSM(0x0D), MSM(0x0E), MSM(0x0F), MSM(0x10), MSM(0x0F), MSM(0x11), MSM(0x12), MSM(0x13), 
		 MSM(0x14), MSM(0x15), MSM(0x16), MSM(0x17), MSM(0x18), MSM(0x11), 
	 0}
};



#define TiM_Logo(x)   ((LOBYTE(Menu_Scetpre_Logo))+x)

 BYTE code strTitLogoWindow[3][31]=
{
	{	 TiM_Logo(0x00), TiM_Logo(0x01), TiM_Logo(0x02), TiM_Logo(0x02), TiM_Logo(0x03), TiM_Logo(0x04), TiM_Logo(0x02), TiM_Logo(0x02), 
		 TiM_Logo(0x05), TiM_Logo(0x06), TiM_Logo(0x07), TiM_Logo(0x02), TiM_Logo(0x08), TiM_Logo(0x02), TiM_Logo(0x02), TiM_Logo(0x09), 
		 TiM_Logo(0x0A), TiM_Logo(0x0B), TiM_Logo(0x0C), TiM_Logo(0x0D), TiM_Logo(0x02), TiM_Logo(0x0E), TiM_Logo(0x02), TiM_Logo(0x02), 
		 TiM_Logo(0x0F), TiM_Logo(0x10), TiM_Logo(0x11), TiM_Logo(0x12), TiM_Logo(0x02), TiM_Logo(0x13), 
	 0},
	{	 TiM_Logo(0x14), TiM_Logo(0x15), TiM_Logo(0x16), TiM_Logo(0x17), TiM_Logo(0x18), TiM_Logo(0x19), TiM_Logo(0x1A), TiM_Logo(0x1A), 
		 TiM_Logo(0x1B), TiM_Logo(0x1C), TiM_Logo(0x1D), TiM_Logo(0x1D), TiM_Logo(0x1E), TiM_Logo(0x1F), TiM_Logo(0x20), TiM_Logo(0x21), 
		 TiM_Logo(0x22), TiM_Logo(0x1A), TiM_Logo(0x23), TiM_Logo(0x24), TiM_Logo(0x1A), TiM_Logo(0x25), TiM_Logo(0x26), TiM_Logo(0x27), 
		 TiM_Logo(0x28), TiM_Logo(0x29), TiM_Logo(0x2A), TiM_Logo(0x1D), TiM_Logo(0x1D), TiM_Logo(0x2B), 
	 0},
	{	 TiM_Logo(0x2C), TiM_Logo(0x2D), TiM_Logo(0x2D), TiM_Logo(0x2E), TiM_Logo(0x2F), TiM_Logo(0x30), TiM_Logo(0x31), TiM_Logo(0x31), 
		 TiM_Logo(0x32), TiM_Logo(0x33), TiM_Logo(0x34), TiM_Logo(0x2D), TiM_Logo(0x35), TiM_Logo(0x36), TiM_Logo(0x1A), TiM_Logo(0x1A), 
		 TiM_Logo(0x1A), TiM_Logo(0x1A), TiM_Logo(0x37), TiM_Logo(0x38), TiM_Logo(0x1A), TiM_Logo(0x39), TiM_Logo(0x3A), TiM_Logo(0x1A), 
		 TiM_Logo(0x3B), TiM_Logo(0x3C), TiM_Logo(0x3D), TiM_Logo(0x3E), TiM_Logo(0x2D), TiM_Logo(0x3F), 
	 0}
};


///////////////////////////////////////////////////////
#define DeM(x)            (MainString2Color_Start+0x63+x)
 BYTE code strDesWindow[2][12]=
{
	{	 DeM(0x00), DeM(0x01), DeM(0x02), DeM(0x03), DeM(0x04), DeM(0x05), DeM(0x06), DeM(0x07), 
		 DeM(0x08), DeM(0x09), DeM(0x0A), 
	 0},
	{	 DeM(0x0B), DeM(0x0C), DeM(0x0D), DeM(0x0E), DeM(0x0F), DeM(0x10), DeM(0x11), DeM(0x12), 
		 DeM(0x13), DeM(0x14), DeM(0x15), 
	 0}
};


#define QuM(x)            (MainString2Color_Start+x)
 BYTE code strQuiWindow[2][18]=
{
	{	 QuM(0x00), QuM(0x01), QuM(0x02), QuM(0x03), QuM(0x04), QuM(0x05), QuM(0x06), QuM(0x07), 
		 QuM(0x08), QuM(0x09), QuM(0x0A), QuM(0x0B), QuM(0x0C), QuM(0x0D), QuM(0x0E), QuM(0x0F), 
		 QuM(0x10), 
	 0},
	{	 QuM(0x11), QuM(0x12), QuM(0x13), QuM(0x14), QuM(0x15), QuM(0x16), QuM(0x17), QuM(0x18), 
		 QuM(0x19), QuM(0x1A), QuM(0x1B), QuM(0x1C), QuM(0x1D), QuM(0x1E), QuM(0x1F), QuM(0x20), 
		 QuM(0x21), 
	 0}
};
#define PiM(x)            (MainString2Color_Start+0x22+x)

 BYTE code strPicWindow[2][18]=
{
	{	 PiM(0x00), PiM(0x00), PiM(0x00), PiM(0x01), PiM(0x02), PiM(0x03), PiM(0x04), PiM(0x05), 
		 PiM(0x06), PiM(0x07), PiM(0x08), PiM(0x09), PiM(0x0A), PiM(0x0B), PiM(0x00), PiM(0x00), 
		 PiM(0x00), 
	 0},
	{	 PiM(0x00), PiM(0x00), PiM(0x00), PiM(0x0C), PiM(0x0D), PiM(0x0E), PiM(0x0F), PiM(0x10), 
		 PiM(0x11), PiM(0x12), PiM(0x13), PiM(0x14), PiM(0x15), PiM(0x16), PiM(0x00), PiM(0x00), 
		 PiM(0x00), 
	 0}
};

#define CoM(x)            (MainString2Color_Start+0x39+x)

 BYTE code strColWindow[2][18]=
{
	{	 CoM(0x00), CoM(0x00), CoM(0x00), CoM(0x00), CoM(0x01), CoM(0x02), CoM(0x03), CoM(0x04), 
		 CoM(0x05), CoM(0x06), CoM(0x07), CoM(0x08), CoM(0x09), CoM(0x00), CoM(0x00), CoM(0x00), 
		 CoM(0x00), 
	 0},
	{	 CoM(0x00), CoM(0x00), CoM(0x00), CoM(0x00), CoM(0x0A), CoM(0x0B), CoM(0x0C), CoM(0x0D), 
		 CoM(0x0E), CoM(0x0F), CoM(0x10), CoM(0x11), CoM(0x12), CoM(0x00), CoM(0x00), CoM(0x00), 
		 CoM(0x00), 
	 0}
};

#define SYM(x)            (MainString2Color_Start+0x4C+x)

 BYTE code strSYSWindow[2][18]=
{
	{	 SYM(0x00), SYM(0x00), SYM(0x00), SYM(0x01), SYM(0x02), SYM(0x03), SYM(0x04), SYM(0x05), 
		 SYM(0x06), SYM(0x07), SYM(0x08), SYM(0x09), SYM(0x0A), SYM(0x0B), SYM(0x00), SYM(0x00), 
		 SYM(0x00), 
	 0},
	{	 SYM(0x00), SYM(0x00), SYM(0x00), SYM(0x0C), SYM(0x0D), SYM(0x0E), SYM(0x0F), SYM(0x10), 
		 SYM(0x11), SYM(0x12), SYM(0x13), SYM(0x14), SYM(0x15), SYM(0x16), SYM(0x00), SYM(0x00), 
		 SYM(0x00), 
	 0}
};


//////////////////////////////////////////////////////////

#define DeM_Span(x)            (MainString2Color_Start+0x69+x)
 BYTE code strDesWindow_Span[2][13]=
{
	{	 DeM_Span(0x00), DeM_Span(0x01), DeM_Span(0x02), DeM_Span(0x03), DeM_Span(0x04), DeM_Span(0x05), DeM_Span(0x06), DeM_Span(0x07), 
		 DeM_Span(0x08), DeM_Span(0x09), DeM_Span(0x0A), DeM_Span(0x0B), 
	 0},
	{	 DeM_Span(0x0C), DeM_Span(0x0D), DeM_Span(0x0E), DeM_Span(0x0F), DeM_Span(0x10), DeM_Span(0x11), DeM_Span(0x12), DeM_Span(0x13), 
		 DeM_Span(0x14), DeM_Span(0x15), DeM_Span(0x16), DeM_Span(0x17), 
	 0}
};


#define QuM_Span(x)            (MainString2Color_Start+x)

 BYTE code strQuiWindow_Span[2][21]=
{
	{	 QuM_Span(0x00), QuM_Span(0x01), QuM_Span(0x02), QuM_Span(0x03), QuM_Span(0x04), QuM_Span(0x05), QuM_Span(0x06), QuM_Span(0x07), 
		 QuM_Span(0x08), QuM_Span(0x09), QuM_Span(0x0A), QuM_Span(0x0B), QuM_Span(0x0C), QuM_Span(0x01), QuM_Span(0x0D), QuM_Span(0x0E), 
		 QuM_Span(0x0F), QuM_Span(0x10), QuM_Span(0x11), QuM_Span(0x12), 
	 0},
	{	 QuM_Span(0x13), QuM_Span(0x14), QuM_Span(0x15), QuM_Span(0x16), QuM_Span(0x17), QuM_Span(0x18), QuM_Span(0x19), QuM_Span(0x1A), 
		 QuM_Span(0x1B), QuM_Span(0x1C), QuM_Span(0x1D), QuM_Span(0x1E), QuM_Span(0x1F), QuM_Span(0x20), QuM_Span(0x21), QuM_Span(0x22), 
		 QuM_Span(0x23), QuM_Span(0x24), QuM_Span(0x25), QuM_Span(0x26), 
	 0}
};
#define PiM_Span(x)            (MainString2Color_Start+0x27+x)

 BYTE code strPicWindow_Span[2][13]=
{
	{	 PiM_Span(0x00), PiM_Span(0x01), PiM_Span(0x02), PiM_Span(0x03), PiM_Span(0x04), PiM_Span(0x05), PiM_Span(0x06), PiM_Span(0x05), 
		 PiM_Span(0x07), PiM_Span(0x08), PiM_Span(0x09), PiM_Span(0x0A), 
	 0},
	{	 PiM_Span(0x0B), PiM_Span(0x0C), PiM_Span(0x0D), PiM_Span(0x0E), PiM_Span(0x0F), PiM_Span(0x10), PiM_Span(0x11), PiM_Span(0x12), 
		 PiM_Span(0x13), PiM_Span(0x14), PiM_Span(0x15), PiM_Span(0x16), 
	 0}
};


#define CoM_Span(x)            (MainString2Color_Start+x+0x3E)

 BYTE code strColWindow_Span[2][18]=
{
	{	 CoM_Span(0x00), CoM_Span(0x00), CoM_Span(0x00), CoM_Span(0x00), CoM_Span(0x01), CoM_Span(0x02), CoM_Span(0x03), CoM_Span(0x04), 
		 CoM_Span(0x05), CoM_Span(0x06), CoM_Span(0x07), CoM_Span(0x08), CoM_Span(0x09), CoM_Span(0x00), CoM_Span(0x00), CoM_Span(0x00), 
		 CoM_Span(0x00), 
	 0},
	{	 CoM_Span(0x00), CoM_Span(0x00), CoM_Span(0x00), CoM_Span(0x00), CoM_Span(0x0A), CoM_Span(0x0B), CoM_Span(0x0C), CoM_Span(0x0D), 
		 CoM_Span(0x0E), CoM_Span(0x0F), CoM_Span(0x10), CoM_Span(0x11), CoM_Span(0x12), CoM_Span(0x00), CoM_Span(0x00), CoM_Span(0x00), 
		 CoM_Span(0x00), 
	 0}
};


#define SYM_Span(x)            (MainString2Color_Start+0x51+x)
 BYTE code strSysWindow_Span[2][13]=
{
	{	 SYM_Span(0x00), SYM_Span(0x01), SYM_Span(0x02), SYM_Span(0x03), SYM_Span(0x04), SYM_Span(0x05), SYM_Span(0x06), SYM_Span(0x07), 
		 SYM_Span(0x08), SYM_Span(0x09), SYM_Span(0x0A), SYM_Span(0x0B), 
	 0},
	{	 SYM_Span(0x0C), SYM_Span(0x0D), SYM_Span(0x0E), SYM_Span(0x0F), SYM_Span(0x10), SYM_Span(0x11), SYM_Span(0x12), SYM_Span(0x13), 
		 SYM_Span(0x14), SYM_Span(0x15), SYM_Span(0x16), SYM_Span(0x17), 
	 0}
};

////////////////////////////////////////////////////////////////


#define DeM_Fran(x)            (MainString2Color_Start+0x65+x)
 BYTE code strDesWindow_Fran[2][12]=
{
	{	 DeM_Fran(0x00), DeM_Fran(0x01), DeM_Fran(0x02), DeM_Fran(0x03), DeM_Fran(0x04), DeM_Fran(0x05), DeM_Fran(0x06), DeM_Fran(0x07), 
		 DeM_Fran(0x08), DeM_Fran(0x09), DeM_Fran(0x0A), 
	 0},
	{	 DeM_Fran(0x0B), DeM_Fran(0x0C), DeM_Fran(0x0D), DeM_Fran(0x0E), DeM_Fran(0x0F), DeM_Fran(0x10), DeM_Fran(0x11), DeM_Fran(0x12), 
		 DeM_Fran(0x13), DeM_Fran(0x14), DeM_Fran(0x15), 
	 0}
};


#define QuM_Fran(x)            (MainString2Color_Start+x)

 BYTE code strQuiWindow_Fran[2][18]=
{
	{	 QuM_Fran(0x00), QuM_Fran(0x01), QuM_Fran(0x02), QuM_Fran(0x03), QuM_Fran(0x04), QuM_Fran(0x05), QuM_Fran(0x06), QuM_Fran(0x07), 
		 QuM_Fran(0x08), QuM_Fran(0x09), QuM_Fran(0x0A), QuM_Fran(0x0B), QuM_Fran(0x0C), QuM_Fran(0x0D), QuM_Fran(0x0E), QuM_Fran(0x0F), 
		 QuM_Fran(0x10), 
	 0},
	{	 QuM_Fran(0x11), QuM_Fran(0x12), QuM_Fran(0x13), QuM_Fran(0x14), QuM_Fran(0x15), QuM_Fran(0x16), QuM_Fran(0x17), QuM_Fran(0x18), 
		 QuM_Fran(0x19), QuM_Fran(0x1A), QuM_Fran(0x1B), QuM_Fran(0x1C), QuM_Fran(0x1D), QuM_Fran(0x1E), QuM_Fran(0x1F), QuM_Fran(0x20), 
		 QuM_Fran(0x21), 
	 0}
};


#define PiM_Fran(x)            (MainString2Color_Start+0x22+x)
 BYTE code strPicWindow_Fran[2][10]=
{
	{	 PiM_Fran(0x00), PiM_Fran(0x01), PiM_Fran(0x02), PiM_Fran(0x03), PiM_Fran(0x04), PiM_Fran(0x05), PiM_Fran(0x06), PiM_Fran(0x07), 
		 PiM_Fran(0x08), 
	 0},
	{	 PiM_Fran(0x09), PiM_Fran(0x0A), PiM_Fran(0x0B), PiM_Fran(0x0C), PiM_Fran(0x0D), PiM_Fran(0x0E), PiM_Fran(0x0F), PiM_Fran(0x10), 
		 PiM_Fran(0x11), 
	 0}
};

#define CoM_Fran(x)            (MainString2Color_Start+0x34+x)
 BYTE code strColWindow_Fran[2][13]=
{
	{	 CoM_Fran(0x00), CoM_Fran(0x01), CoM_Fran(0x02), CoM_Fran(0x03), CoM_Fran(0x04), CoM_Fran(0x05), CoM_Fran(0x06), CoM_Fran(0x07), 
		 CoM_Fran(0x08), CoM_Fran(0x09), CoM_Fran(0x09), CoM_Fran(0x0A), 
	 0},
	{	 CoM_Fran(0x0B), CoM_Fran(0x0C), CoM_Fran(0x0D), CoM_Fran(0x0E), CoM_Fran(0x0F), CoM_Fran(0x10), CoM_Fran(0x11), CoM_Fran(0x12), 
		 CoM_Fran(0x13), CoM_Fran(0x14), CoM_Fran(0x15), CoM_Fran(0x16), 
	 0}
};

#define SyM_Fran(x)            (MainString2Color_Start+0x4B+x)

 BYTE code strSysWindow_Fran[2][14]=
{
	{	 SyM_Fran(0x00), SyM_Fran(0x01), SyM_Fran(0x02), SyM_Fran(0x03), SyM_Fran(0x04), SyM_Fran(0x05), SyM_Fran(0x06), SyM_Fran(0x07), 
		 SyM_Fran(0x08), SyM_Fran(0x09), SyM_Fran(0x0A), SyM_Fran(0x0B), SyM_Fran(0x0C), 
	 0},
	{	 SyM_Fran(0x0D), SyM_Fran(0x0E), SyM_Fran(0x0F), SyM_Fran(0x10), SyM_Fran(0x11), SyM_Fran(0x12), SyM_Fran(0x13), SyM_Fran(0x14), 
		 SyM_Fran(0x15), SyM_Fran(0x16), SyM_Fran(0x17), SyM_Fran(0x18), SyM_Fran(0x19), 
	 0}
};

//////////////////////////////////////////////////////////

#define DeM_TChinese(x)            (MainString2Color_Start+x+0x3D)
 BYTE code strDesWindow_TChinese[2][6]=
{
	{	 DeM_TChinese(0x00), DeM_TChinese(0x01), DeM_TChinese(0x02), DeM_TChinese(0x03), DeM_TChinese(0x04), 
	 0},
	{	 DeM_TChinese(0x05), DeM_TChinese(0x06), DeM_TChinese(0x07), DeM_TChinese(0x08), DeM_TChinese(0x09), 
	 0}
};


#define QuM_TChinese(x)            (MainString2Color_Start+x)

 BYTE code strQuiWindow_TChinese[2][11]=
{
	{	 QuM_TChinese(0x00), QuM_TChinese(0x01), QuM_TChinese(0x02), QuM_TChinese(0x03), QuM_TChinese(0x04), QuM_TChinese(0x05), QuM_TChinese(0x06), QuM_TChinese(0x07), 
		 QuM_TChinese(0x08), QuM_TChinese(0x09), 
	 0},
	{	 QuM_TChinese(0x0A), QuM_TChinese(0x0B), QuM_TChinese(0x0C), QuM_TChinese(0x0D), QuM_TChinese(0x0E), QuM_TChinese(0x0F), QuM_TChinese(0x10), QuM_TChinese(0x11), 
		 QuM_TChinese(0x12), QuM_TChinese(0x13), 
	 0}
};
#define PiM_TChinese(x)            (MainString2Color_Start+0x14+x)

 BYTE code strPicWindow_TChinese[2][8]=
{
	{	 PiM_TChinese(0x00), PiM_TChinese(0x01), PiM_TChinese(0x02), PiM_TChinese(0x03), PiM_TChinese(0x04), PiM_TChinese(0x05), PiM_TChinese(0x06), 
	 0},
	{	 PiM_TChinese(0x07), PiM_TChinese(0x08), PiM_TChinese(0x08), PiM_TChinese(0x09), PiM_TChinese(0x0A), PiM_TChinese(0x0B), PiM_TChinese(0x0C), 
	 0}
};


#define CoM_TChinese(x)            (MainString2Color_Start+x+0x21)

 BYTE code strColWindow_TChinese[2][8]=
{
	{	 CoM_TChinese(0x00), CoM_TChinese(0x01), CoM_TChinese(0x02), CoM_TChinese(0x03), CoM_TChinese(0x04), CoM_TChinese(0x05), CoM_TChinese(0x06), 
	 0},
	{	 CoM_TChinese(0x07), CoM_TChinese(0x08), CoM_TChinese(0x09), CoM_TChinese(0x0A), CoM_TChinese(0x0B), CoM_TChinese(0x0C), CoM_TChinese(0x0D), 
	 0}
};


#define SYM_TChinese(x)            (MainString2Color_Start+0x2F+x)
 BYTE code strSysWindow_TChinese[2][8]=
{
	{	 SYM_TChinese(0x00), SYM_TChinese(0x01), SYM_TChinese(0x02), SYM_TChinese(0x03), SYM_TChinese(0x04), SYM_TChinese(0x05), SYM_TChinese(0x06), 
	 0},
	{	 SYM_TChinese(0x07), SYM_TChinese(0x08), SYM_TChinese(0x09), SYM_TChinese(0x0A), SYM_TChinese(0x0B), SYM_TChinese(0x0C), SYM_TChinese(0x0D), 
	 0}
};


///////////////////////////////////////////////////////////////////////////
//Sub Icon Menu

#define SubQu(x)           (MM_4COLOR_Icon_START+0x02*x)        
 BYTE code strSubQuWindow[2][5]=
{
	{	 SubQu(0x00), SubQu(0x01), SubQu(0x02), SubQu(0x03), 
	 0},
	{	 SubQu(0x04), SubQu(0x05), SubQu(0x06), SubQu(0x07), 
	 0}
};



#define SubPic(x)           (MM_4COLOR_Icon_START+8*2+0x02*x)        
#define SuEPic(x)           (MM_8COLOR_ICON_START+0x03*x)        
 BYTE code strSubPicWindow[2][5]=
{
	{	 SubPic(0x00), SuEPic(0x00), SuEPic(0x01), SubPic(0x01), 
	 0},
	{	 SubPic(0x02), SuEPic(0x02), SuEPic(0x03), SubPic(0x03), 
	 0}
};

#define SubCol(x)           (MM_4COLOR_Icon_START+12*2+0x02*x)        
 BYTE code strSubColWindow[2][5]=
{
	{	 SubCol(0x00), SubCol(0x01), SubCol(0x02), SubCol(0x03), 
	 0},
	{	 SubCol(0x04), SubCol(0x05), SubCol(0x06), SubCol(0x07), 
	 0}
};

#define SubSys(x)           (MM_4COLOR_Icon_START+20*2+0x02*x)        
 BYTE code strSubSysWindow[2][5]=
{
	{	 SubSys(0x00), SubSys(0x01), SubSys(0x02), SubSys(0x03), 
	 0},
	{	 SubSys(0x04), SubSys(0x05), SubSys(0x06), SubSys(0x07), 
	 0}
};

#define VGM(x)           (MainString2Color_Start+0x79+x)        

 BYTE code strVGAWindow[2][21]=
{
	{	 VGM(0x00), VGM(0x01), VGM(0x02), VGM(0x03), VGM(0x04), VGM(0x05), VGM(0x06), VGM(0x07), 
		 VGM(0x08), VGM(0x09), VGM(0x0A), VGM(0x0B), VGM(0x0C), VGM(0x0D), VGM(0x0E), VGM(0x0F), 
		 VGM(0x10), VGM(0x11), VGM(0x12), VGM(0x13), 
	 0},
	{	 VGM(0x14), VGM(0x15), VGM(0x16), VGM(0x17), VGM(0x18), VGM(0x19), VGM(0x1A), VGM(0x1B), 
		 VGM(0x1C), VGM(0x1D), VGM(0x1E), VGM(0x1F), VGM(0x20), VGM(0x21), VGM(0x22), VGM(0x23), 
		 VGM(0x24), VGM(0x25), VGM(0x26), VGM(0x27), 
	 0}
};
