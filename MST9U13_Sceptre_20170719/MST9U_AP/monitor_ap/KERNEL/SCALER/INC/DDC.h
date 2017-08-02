extern void mStar_WriteDDC1( void );
extern void mStar_WriteDDC2( void );

#ifdef UseInternalDDCRam
typedef enum
{
    VGA_EDID,
    DVI_EDID
} EDIDType;


extern void mStar_InitDDC( void );
extern void mStar_CheckRAM( void );
extern void mStar_ClrDDC_WP( void );
extern void mStar_SetDDC_WP( void );
extern void ReLoadHDMI_EDID(void);

#endif
