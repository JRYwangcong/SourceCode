#ifndef FactoryAligment_H
#define FactoryAligment_H


extern Bool WistronAlignControl(void);
#define ADJ_Screen_Pattern          0x53
#define GET_VCP_PAGE                0xC4
#define ASSET_READ_Wistron          0x99 //20090429
#define ASSET_WRITE_Wistron         0x55
#define ASSET_Serial_Number         0xAA
#define ASSET_Backlight_Hours       0xBB
#define ASSET_Firmware_Version      0xCC
#define ASSET_Model_Name            0xDD
#define ASSET_Firmware_PIN          0xEE
#define ADJ_Firmware_Version        0x51
#define	DDC2B_DEST_ADDRESS          0x6e
//#define ADJ_EDID_WP               0xF2 // mhc, 20130412
#define ADJ_VOLUME                  0x62
#define ADJ_AGINGMODE               0x77
#define ADJ_OSD_LANGUAGE            0xCC

#define ADJ_EDID_WP                 0xF2

#endif//FactoryAligment_H
