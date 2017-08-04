#ifndef _MSMINILVDS_H_
#define  _MSMINILVDS_H_

#ifdef _MSMINILVDS_C_
  #define _MSMINILVDSDEC_
#else
  #define _MSMINILVDSDEC_ extern
#endif

#if VCOM_TYPE == VCOM_MAXIM		//120322 Modify
_MSMINILVDSDEC_ Bool msMiniLVDSi2c_Write2Byte(BYTE add, WORD val);
_MSMINILVDSDEC_ Bool msMiniLVDSi2c_Read2Byte( BYTE add, WORD *val );
_MSMINILVDSDEC_ Bool msWriteMiniLVDSi2c_Vcom_DAC(WORD ucVcom);
_MSMINILVDSDEC_ Bool msWriteMiniLVDSi2c_Vcom(WORD ucVcom);
_MSMINILVDSDEC_ WORD msReadMiniLVDSi2c_Vcom(void);
//_MSMINILVDSDEC_ WORD msReadMiniLVDSi2c_Vcom_DAC(void);
#else
_MSMINILVDSDEC_ Bool msMiniLVDSi2c_WriteByte(BYTE add, BYTE val);
_MSMINILVDSDEC_ Bool msMiniLVDSi2c_ReadByte( BYTE add, BYTE *val );

_MSMINILVDSDEC_ BYTE msReadMiniLVDSi2c_Vcom(void);
_MSMINILVDSDEC_ Bool msWriteMiniLVDSi2c_Vcom(BYTE ucVcom);
_MSMINILVDSDEC_ Bool msWriteMiniLVDSi2c_Vcom_DAC(BYTE ucVcom);
#endif
#endif
