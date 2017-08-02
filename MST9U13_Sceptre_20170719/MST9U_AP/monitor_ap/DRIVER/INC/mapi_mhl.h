///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   Mapi_mhl.h
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
    1. Check whether MHL cable is plugged or not in Rx side
    2. Disable R-term and output Vbus
    3. Check whether Cbus is connected(discovery pulse completed)
    4. Sink -> Source: "SET_HPD"
    5. Sink -> Source: Write_Stat - "DCAP_RDY" and Write_INT - "DCAP_CHG"
    6. Sink -> Source: Write_Stat - "PATH_EN"
*/


#ifndef _MAPI_MHL_H_
#define _MAPI_MHL_H_

#include "combo_config.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MHL_DEBUG_MESSAGE               (1&ENABLE_MSTV_UART_DEBUG)

#define MHL_IP_SUPPORT_0PORT            0
#define MHL_IP_SUPPORT_1PORT            1
#define MHL_IP_SUPPORT_2PORT            2
#define MHL_IP_SUPPORT_3PORT            3
#define MHL_IP_SUPPORT_4PORT            4
#define MHL_IP_SUPPORT_5PORT            5
#define MHL_IP_SUPPORT_6PORT            6

#define MHL_VENDOR_ID                   0x00 // ?
#define MHL_ADOPTER_ID                  0x02CB // Mstar: 715
#define MHL_DEVICE_ID                   0x0000 // Chip ID?
#define MHL_RCP_SUPPORT                 1   // For CTS1.2 must be set
#define MHL_RAP_SUPPORT                 1   // For CTS1.2 must be set
#define MHL_DEFAULT_2CLASS_VERSION      0x21
#define MHL_DEFAULT_3CLASS_VERSION      0x32

#define MHL_FUNCTION_SUPPORT_PORT0      COMBO_MHL_SUPPORT_PORT0
#define MHL_FUNCTION_SUPPORT_PORT1      COMBO_MHL_SUPPORT_PORT1
#define MHL_FUNCTION_SUPPORT_PORT2      COMBO_MHL_SUPPORT_PORT2
#define MHL_FUNCTION_SUPPORT_PORT3      COMBO_MHL_SUPPORT_PORT3
#define MHL_FUNCTION_SUPPORT_PORT4      COMBO_MHL_SUPPORT_PORT4
#define MHL_FUNCTION_SUPPORT_PORT5      COMBO_MHL_SUPPORT_PORT5

#define MHL_SUPPORT_MHL3_PORT0          COMBO_MHL3_SUPPORT_PORT0
#define MHL_SUPPORT_MHL3_PORT1          COMBO_MHL3_SUPPORT_PORT1
#define MHL_SUPPORT_MHL3_PORT2          COMBO_MHL3_SUPPORT_PORT2
#define MHL_SUPPORT_MHL3_PORT3          COMBO_MHL3_SUPPORT_PORT3
#define MHL_SUPPORT_MHL3_PORT4          COMBO_MHL3_SUPPORT_PORT4
#define MHL_SUPPORT_MHL3_PORT5          COMBO_MHL3_SUPPORT_PORT5

#define MHL_IP_SUPPORT_TYPE             COMBO_IP_SUPPORT_TYPE

//-------------------------------------------------------------------------------------------------
//  Interface Link
//-------------------------------------------------------------------------------------------------
#define MHL_DEFINE_COMBO_IP_PORT0       COMBO_INPUT_SELECT_PORT0
#define MHL_DEFINE_COMBO_IP_PORT1       COMBO_INPUT_SELECT_PORT1
#define MHL_DEFINE_COMBO_IP_PORT2       COMBO_INPUT_SELECT_PORT2
#define MHL_DEFINE_COMBO_IP_PORT3       COMBO_INPUT_SELECT_PORT3
#define MHL_DEFINE_COMBO_IP_PORT4       COMBO_INPUT_SELECT_PORT4
#define MHL_DEFINE_COMBO_IP_PORT5       COMBO_INPUT_SELECT_PORT5

#if(MHL_FUNCTION_SUPPORT_PORT0)
#define MHL_CABLE_SAR_DETECT_PORT0      hwDigital0Cable_Pin
#define SET_MHL_VBUS_CHARGE_PORT0()     SET_MHL_PS_CTRL_CHARGE0();
#define CLR_MHL_VBUS_CHARGE_PORT0()     CLR_MHL_PS_CTRL_CHARGE0();
#define SET_MHL_HDMI_HPD_PORT0()        hw_Set_HdcpHpd();
#define CLR_MHL_HDMI_HPD_PORT0()        hw_Clr_HdcpHpd();

#ifdef XDATA_MHL_CBUS_START0
#define MHL_CBUS_SRAM_ADDRESS_PORT0     XDATA_MHL_CBUS_START0
#else
#define MHL_CBUS_SRAM_ADDRESS_PORT0     0
#endif

#else
#define MHL_CABLE_SAR_DETECT_PORT0      
#define SET_MHL_VBUS_CHARGE_PORT0()     
#define CLR_MHL_VBUS_CHARGE_PORT0()     
#define SET_MHL_HDMI_HPD_PORT0()        
#define CLR_MHL_HDMI_HPD_PORT0()        
#endif

#if(MHL_FUNCTION_SUPPORT_PORT1)
#define MHL_CABLE_SAR_DETECT_PORT1      hwDigital1Cable_Pin
#define SET_MHL_VBUS_CHARGE_PORT1()     SET_MHL_PS_CTRL_CHARGE1();
#define CLR_MHL_VBUS_CHARGE_PORT1()     CLR_MHL_PS_CTRL_CHARGE1();
#define SET_MHL_HDMI_HPD_PORT1()        hw_Set_HdcpHpd1();
#define CLR_MHL_HDMI_HPD_PORT1()        hw_Clr_HdcpHpd1();

#ifdef XDATA_MHL_CBUS_START1
#define MHL_CBUS_SRAM_ADDRESS_PORT1     XDATA_MHL_CBUS_START1
#else
#define MHL_CBUS_SRAM_ADDRESS_PORT1     0
#endif

#else
#define MHL_CABLE_SAR_DETECT_PORT1      
#define SET_MHL_VBUS_CHARGE_PORT1()     
#define CLR_MHL_VBUS_CHARGE_PORT1()     
#define SET_MHL_HDMI_HPD_PORT1()        
#define CLR_MHL_HDMI_HPD_PORT1()        
#endif

#if(MHL_FUNCTION_SUPPORT_PORT2)
#define MHL_CABLE_SAR_DETECT_PORT2      hwDigital2Cable_Pin
#define SET_MHL_VBUS_CHARGE_PORT2()     SET_MHL_PS_CTRL_CHARGE2();
#define CLR_MHL_VBUS_CHARGE_PORT2()     CLR_MHL_PS_CTRL_CHARGE2();
#define SET_MHL_HDMI_HPD_PORT2()        hw_Set_HdcpHpd2();
#define CLR_MHL_HDMI_HPD_PORT2()        hw_Clr_HdcpHpd2();

#ifdef XDATA_MHL_CBUS_START2
#define MHL_CBUS_SRAM_ADDRESS_PORT2     XDATA_MHL_CBUS_START2
#else
#define MHL_CBUS_SRAM_ADDRESS_PORT2     0
#endif

#else
#define MHL_CABLE_SAR_DETECT_PORT2      
#define SET_MHL_VBUS_CHARGE_PORT2()     
#define CLR_MHL_VBUS_CHARGE_PORT2()     
#define SET_MHL_HDMI_HPD_PORT2()        
#define CLR_MHL_HDMI_HPD_PORT2()        
#endif

#if(MHL_FUNCTION_SUPPORT_PORT3)
#define MHL_CABLE_SAR_DETECT_PORT3      hwDigital3Cable_Pin
#define SET_MHL_VBUS_CHARGE_PORT3()     SET_MHL_PS_CTRL_CHARGE3();
#define CLR_MHL_VBUS_CHARGE_PORT3()     CLR_MHL_PS_CTRL_CHARGE3();
#define SET_MHL_HDMI_HPD_PORT3()        hw_Set_HdcpHpd3();
#define CLR_MHL_HDMI_HPD_PORT3()        hw_Clr_HdcpHpd3();

#ifdef XDATA_MHL_CBUS_START3
#define MHL_CBUS_SRAM_ADDRESS_PORT3     XDATA_MHL_CBUS_START3
#else
#define MHL_CBUS_SRAM_ADDRESS_PORT3     0
#endif

#else
#define MHL_CABLE_SAR_DETECT_PORT3      
#define SET_MHL_VBUS_CHARGE_PORT3()     
#define CLR_MHL_VBUS_CHARGE_PORT3()     
#define SET_MHL_HDMI_HPD_PORT3()        
#define CLR_MHL_HDMI_HPD_PORT3()        
#endif

#if(MHL_FUNCTION_SUPPORT_PORT4)
#define MHL_CABLE_SAR_DETECT_PORT4      hwDigital4Cable_Pin
#define SET_MHL_VBUS_CHARGE_PORT4()     SET_MHL_PS_CTRL_CHARGE4();
#define CLR_MHL_VBUS_CHARGE_PORT4()     CLR_MHL_PS_CTRL_CHARGE4();
#define SET_MHL_HDMI_HPD_PORT4()        hw_Set_HdcpHpd4();
#define CLR_MHL_HDMI_HPD_PORT4()        hw_Clr_HdcpHpd4();

#ifdef XDATA_MHL_CBUS_START4
#define MHL_CBUS_SRAM_ADDRESS_PORT4     XDATA_MHL_CBUS_START4
#else
#define MHL_CBUS_SRAM_ADDRESS_PORT4     0
#endif

#else
#define MHL_CABLE_SAR_DETECT_PORT4      
#define SET_MHL_VBUS_CHARGE_PORT4()     
#define CLR_MHL_VBUS_CHARGE_PORT4()     
#define SET_MHL_HDMI_HPD_PORT4()        
#define CLR_MHL_HDMI_HPD_PORT4()        
#endif

#if(MHL_FUNCTION_SUPPORT_PORT5)
#define MHL_CABLE_SAR_DETECT_PORT5      hwDigital5Cable_Pin
#define SET_MHL_VBUS_CHARGE_PORT5()     SET_MHL_PS_CTRL_CHARGE5();
#define CLR_MHL_VBUS_CHARGE_PORT5()     CLR_MHL_PS_CTRL_CHARGE5();
#define SET_MHL_HDMI_HPD_PORT5()        hw_Set_HdcpHpd5();
#define CLR_MHL_HDMI_HPD_PORT5()        hw_Clr_HdcpHpd5();

#ifdef XDATA_MHL_CBUS_START5
#define MHL_CBUS_SRAM_ADDRESS_PORT5     XDATA_MHL_CBUS_START5
#else
#define MHL_CBUS_SRAM_ADDRESS_PORT5     0
#endif

#else
#define MHL_CABLE_SAR_DETECT_PORT5      
#define SET_MHL_VBUS_CHARGE_PORT5()     
#define CLR_MHL_VBUS_CHARGE_PORT5()     
#define SET_MHL_HDMI_HPD_PORT5()        
#define CLR_MHL_HDMI_HPD_PORT5()        
#endif

#define MHL_HDMI14_B_CHANNEL_EQ         COMBO_HDMI14_B_CHANNEL_EQ
#define MHL_HDMI14_G_CHANNEL_EQ         COMBO_HDMI14_G_CHANNEL_EQ
#define MHL_HDMI14_R_CHANNEL_EQ         COMBO_HDMI14_R_CHANNEL_EQ

#define MHL_HDMI20_B_CHANNEL_EQ         COMBO_HDMI20_B_CHANNEL_EQ
#define MHL_HDMI20_G_CHANNEL_EQ         COMBO_HDMI20_G_CHANNEL_EQ
#define MHL_HDMI20_R_CHANNEL_EQ         COMBO_HDMI20_R_CHANNEL_EQ

#define MHL_RCONTROL_OVERWRITE_PORT0    0
#define MHL_RCONTROL_OVERWRITE_PORT1    0
#define MHL_RCONTROL_OVERWRITE_PORT2    0
#define MHL_RCONTROL_OVERWRITE_PORT3    0
#define MHL_RCONTROL_OVERWRITE_PORT4    0
#define MHL_RCONTROL_OVERWRITE_PORT5    0

#if(MHL_RCONTROL_OVERWRITE_PORT0 > 0)
#define MHL_RCONTROL_OVERWRITE_ENABLE0  1
#else
#define MHL_RCONTROL_OVERWRITE_ENABLE0  0
#endif

#if(MHL_RCONTROL_OVERWRITE_PORT1 > 0)
#define MHL_RCONTROL_OVERWRITE_ENABLE1  1
#else
#define MHL_RCONTROL_OVERWRITE_ENABLE1  0
#endif

#if(MHL_RCONTROL_OVERWRITE_PORT2 > 0)
#define MHL_RCONTROL_OVERWRITE_ENABLE2  1
#else
#define MHL_RCONTROL_OVERWRITE_ENABLE2  0
#endif

#if(MHL_RCONTROL_OVERWRITE_PORT3 > 0)
#define MHL_RCONTROL_OVERWRITE_ENABLE3  1
#else
#define MHL_RCONTROL_OVERWRITE_ENABLE3  0
#endif

#if(MHL_RCONTROL_OVERWRITE_PORT4 > 0)
#define MHL_RCONTROL_OVERWRITE_ENABLE4  1
#else
#define MHL_RCONTROL_OVERWRITE_ENABLE4  0
#endif

#if(MHL_RCONTROL_OVERWRITE_PORT5 > 0)
#define MHL_RCONTROL_OVERWRITE_ENABLE5  1
#else
#define MHL_RCONTROL_OVERWRITE_ENABLE5  0
#endif

#define MHL_USER_DEVICE_CAPABILITY      0   // User define device capacibility
#define MHL_READ_DEVICE_CAPABILITY      1   // Read source device capacibility
#define MHL_READ_DEVICE_VENDER_ID       0   // Read source device vender ID
#define MHL_GET_CABLE_DETECT_SUPPORT    1
#define MHL_HW_RTERM_CONTROL_SUPPORT    0
#define MHL_SEND_RCP_FUNCTION_SUPPORT   0
#define MHL_SEND_RAP_FUNCTION_SUPPORT   0
#define MHL_SEND_UCP_FUNCTION_SUPPORT   0
#define MHL_SEND_WRITE_BURST_SUPPORT    0
#define MHL_3D_TIMING_DISPLAY_SUPPORT   0
#define MHL_HID_FUNCTION_SUPPORT        0
#define MHL_HTC_BUFFER_FLY_CHARGE_PATCH 0
#define MHL_HTC_SENSATION_DC_PATCH 0//no wake up behavior in DC off/on
#define MHL_SAM_SII_SELF_WAKEUP_PATCH 0//SELF wake up for samsung SII by switch port
#define MHL_CBUS_SELF_TEST_SUPPORT      0
#define MHL_SONY_Z3_WITHOUT_DCAP_RDY_PATCH 1 //can`t send DCAP_RDY of cmd before TX to read DCAP

#if(MHL_USER_DEVICE_CAPABILITY)
//-------------------------------------------------------------------------------------------------
//  User define device capacibility
//-------------------------------------------------------------------------------------------------
#define MHL_DEVICE_STATE                0x00
#define MHL_VER_MAJOR_PORT0             2
#define MHL_VER_MINOR_PORT0             1
#define MHL_VER_MAJOR_PORT1             2
#define MHL_VER_MINOR_PORT1             1
#define MHL_VER_MAJOR_PORT2             2
#define MHL_VER_MINOR_PORT2             1
#define MHL_VER_MAJOR_PORT3             2
#define MHL_VER_MINOR_PORT3             1
#define MHL_VER_MAJOR_PORT4             2
#define MHL_VER_MINOR_PORT4             1
#define MHL_VER_MAJOR_PORT5             2
#define MHL_VER_MINOR_PORT5             1
#define MHL_DEVICE_TYPE                 1   // 1: Sink, 2: Source, 3: Dongle, 5: Direct attach Sink, 6: Direct attach Source
#define MHL_DEVICE_POW                  1
#define MHL_DEVICE_PLIM                 1   // 0: 500mA, 1: 900mA, 2: 1.5A, 3: Dongle to source, 4: 2A
#define MHL_SUPP_RGB444                 1
#define MHL_SUPP_YCBCR444               1
#define MHL_SUPP_YCBCR422               1
#define MHL_SUPP_PPIXEL                 1
#define MHL_SUPP_ISLANDS                1
#define MHL_SUPP_VGA                    1
#define MHL_SUPP_16BPP                  1
#define MHL_AUD_2CH                     1
#define MHL_AUD_8CH                     0
#define MHL_AUD_HBR                     0
#define MHL_VT_GRAPHICS                 0
#define MHL_VT_PHOTO                    0
#define MHL_VT_CINEMA                   0
#define MHL_VT_GAME                     0
#define MHL_SUPP_VT                     0
#define MHL_LD_DISPLAY                  1
#define MHL_LD_VIDEO                    0
#define MHL_LD_AUDIO                    0
#define MHL_LD_MEDIA                    0
#define MHL_LD_TUNER                    0
#define MHL_LD_RECORD                   0
#define MHL_LD_SPEAKER                  0
#define MHL_LD_GUI                      0
#define MHL_BANDWIDTH                   0x0F
#define MHL_SP_SUPPORT                  1
#define MHL_UCP_SEND_SUPPORT            0
#define MHL_UCP_RECV_SUPPORT            0
#define MHL_XVYCC_SUPPORT               0
#define MHL_RBP_SUPPORT                 0
#define MHL_SCRATCHPAD_SIZE             0x10
#define MHL_INT_SIZE                    3
#define MHL_STATE_SIZE                  3
#define MHL_ECBUS_S_075                 1
#define MHL_ECBUS_S_8BIT                1
#define MHL_ECBUS_S_12BIT               0
#define MHL_ECBUS_D_150                 0
#define MHL_ECBUS_D_8BIT                0
#define MHL_TMDS_150                    1
#define MHL_TMDS_300                    1
#define MHL_TMDS_600                    1
#define MHL_DEV_HOST                    0
#define MHL_DEV_DEVICE                  1
#define MHL_DEV_CHARGER                 0
#define MHL_HID_HOST                    0
#define MHL_HID_DEVICE                  0
#define MHL_LD_PHONE                    0

#define MHL_DEV_STATE_0x00              MHL_DEVICE_STATE
#define MHL_VERSION_0x01_PORT0          (MHL_VER_MINOR_PORT0 |(MHL_VER_MAJOR_PORT0 <<4))
#define MHL_VERSION_0x01_PORT1          (MHL_VER_MINOR_PORT1 |(MHL_VER_MAJOR_PORT1 <<4))
#define MHL_VERSION_0x01_PORT2          (MHL_VER_MINOR_PORT2 |(MHL_VER_MAJOR_PORT2 <<4))
#define MHL_VERSION_0x01_PORT3          (MHL_VER_MINOR_PORT3 |(MHL_VER_MAJOR_PORT3 <<4))
#define MHL_VERSION_0x01_PORT4          (MHL_VER_MINOR_PORT4 |(MHL_VER_MAJOR_PORT4 <<4))
#define MHL_VERSION_0x01_PORT5          (MHL_VER_MINOR_PORT5 |(MHL_VER_MAJOR_PORT5 <<4))
#define MHL_DEV_CAT_0x02                (MHL_DEVICE_TYPE |(MHL_DEVICE_POW <<4)| (MHL_DEVICE_PLIM <<5))
#define MHL_ADOPTER_ID_H_0x03           (MHL_ADOPTER_ID >>8)
#define MHL_ADOPTER_ID_L_0x04           (MHL_ADOPTER_ID &0xFF)
#define MHL_VID_LINK_MODE_0x05          (MHL_SUPP_RGB444 |(MHL_SUPP_YCBCR444 <<1) |(MHL_SUPP_YCBCR422 <<2) |(MHL_SUPP_PPIXEL <<3) |(MHL_SUPP_ISLANDS <<4) |(MHL_SUPP_VGA <<5) |(MHL_SUPP_16BPP <<6))
#define MHL_AUD_LINK_MODE_0x06          (MHL_AUD_2CH |(MHL_AUD_8CH <<1) |(MHL_AUD_HBR <<2))
#define MHL_VIDEO_TYPE_0x07             (MHL_VT_GRAPHICS |(MHL_VT_PHOTO <<1) |(MHL_VT_CINEMA <<2) |(MHL_VT_GAME <<3) |(MHL_SUPP_VT <<7))
#define MHL_LOG_DEV_MAP_0x08            (MHL_LD_DISPLAY |(MHL_LD_VIDEO <<1) |(MHL_LD_AUDIO <<2) |(MHL_LD_MEDIA <<3) |(MHL_LD_TUNER <<4) |(MHL_LD_RECORD <<5) |(MHL_LD_SPEAKER <<6) |(MHL_LD_GUI <<7))
#define MHL_BANDWIDTH_0x09              MHL_BANDWIDTH
#define MHL_FEATURE_FLAG_0x0A           (MHL_RCP_SUPPORT |(MHL_RAP_SUPPORT <<1) |(MHL_SP_SUPPORT <<2) |(MHL_UCP_SEND_SUPPORT <<3) |(MHL_UCP_RECV_SUPPORT <<4) |(MHL_XVYCC_SUPPORT <<5) |(MHL_RBP_SUPPORT <<6))
#define MHL_DEVICE_ID_H_0x0B            (MHL_DEVICE_ID >>8)
#define MHL_DEVICE_ID_L_0x0C            (MHL_DEVICE_ID &0xFF)
#define MHL_SCRATCHPAD_SIZE_0x0D        MHL_SCRATCHPAD_SIZE
#define MHL_INT_STAT_0x0E               (MHL_INT_SIZE |(MHL_STATE_SIZE <<4))
#define MHL_RESERVED_0x0F               0x00
#define MHL_ECBUS_SPEEDS_0x80           (MHL_ECBUS_S_075 |(MHL_ECBUS_S_8BIT <<1) |(MHL_ECBUS_S_12BIT <<2) |(MHL_ECBUS_D_150 <<4) |(MHL_ECBUS_D_8BIT <<5))
#define MHL_TMDS_SPEEDS_0x81            (MHL_TMDS_150 |(MHL_TMDS_300 <<1) |(MHL_TMDS_600 <<2))
#define MHL_ECBUS_DEV_ROLES_0x82        (MHL_DEV_HOST |(MHL_DEV_DEVICE <<1) |(MHL_DEV_CHARGER <<2) |(MHL_HID_HOST <<3) |(MHL_HID_DEVICE <<4))
#define MHL_LOG_DEV_MAPX_0x83           (MHL_LD_PHONE)

#else
#define MHL_DEV_STATE_0x00              0x00
#define MHL_VERSION_0x01_PORT0          (MHL_SUPPORT_MHL3_PORT0? MHL_DEFAULT_3CLASS_VERSION: MHL_DEFAULT_2CLASS_VERSION)
#define MHL_VERSION_0x01_PORT1          (MHL_SUPPORT_MHL3_PORT1? MHL_DEFAULT_3CLASS_VERSION: MHL_DEFAULT_2CLASS_VERSION)
#define MHL_VERSION_0x01_PORT2          (MHL_SUPPORT_MHL3_PORT2? MHL_DEFAULT_3CLASS_VERSION: MHL_DEFAULT_2CLASS_VERSION)
#define MHL_VERSION_0x01_PORT3          (MHL_SUPPORT_MHL3_PORT3? MHL_DEFAULT_3CLASS_VERSION: MHL_DEFAULT_2CLASS_VERSION)
#define MHL_VERSION_0x01_PORT4          (MHL_SUPPORT_MHL3_PORT4? MHL_DEFAULT_3CLASS_VERSION: MHL_DEFAULT_2CLASS_VERSION)
#define MHL_VERSION_0x01_PORT5          (MHL_SUPPORT_MHL3_PORT5? MHL_DEFAULT_3CLASS_VERSION: MHL_DEFAULT_2CLASS_VERSION)
#define MHL_DEV_CAT_0x02                0x31
#define MHL_ADOPTER_ID_H_0x03           (MHL_ADOPTER_ID >>8)
#define MHL_ADOPTER_ID_L_0x04           (MHL_ADOPTER_ID &0xFF)
#define MHL_VID_LINK_MODE_0x05          0x7F //0x37
#define MHL_AUD_LINK_MODE_0x06          0x01
#define MHL_VIDEO_TYPE_0x07             0x00
#define MHL_LOG_DEV_MAP_0x08            0x01
#define MHL_BANDWIDTH_0x09              0x0F
#define MHL_FEATURE_FLAG_0x0A           0x07
#define MHL_DEVICE_ID_H_0x0B            (MHL_DEVICE_ID >>8)
#define MHL_DEVICE_ID_L_0x0C            (MHL_DEVICE_ID &0xFF)
#define MHL_SCRATCHPAD_SIZE_0x0D        0x10
#define MHL_INT_STAT_0x0E               0x33
#define MHL_RESERVED_0x0F               0x00
#define MHL_ECBUS_SPEEDS_0x80           0x03
#define MHL_TMDS_SPEEDS_0x81            0x07
#define MHL_ECBUS_DEV_ROLES_0x82        0x02
#define MHL_LOG_DEV_MAPX_0x83           0x00

#endif

//-------------------------------------------------------------------------------------------------
//  Read device capacibility
//-------------------------------------------------------------------------------------------------
#define MHL_READ_DEVCAP_0x00            1   // Device state
#define MHL_READ_DEVCAP_0x01            1   // MHL version
#define MHL_READ_DEVCAP_0x02            1   // Device category
#define MHL_READ_DEVCAP_0x03            1   // Adopter ID high byte
#define MHL_READ_DEVCAP_0x04            1   // Adopter ID low byte
#define MHL_READ_DEVCAP_0x05            1   // Video link mode support
#define MHL_READ_DEVCAP_0x06            1   // Audio link mode support
#define MHL_READ_DEVCAP_0x07            1   // Video type support
#define MHL_READ_DEVCAP_0x08            1   // Logical device map
#define MHL_READ_DEVCAP_0x09            1   // Link bandwidth limit
#define MHL_READ_DEVCAP_0x0A            1   // Feature flags
#define MHL_READ_DEVCAP_0x0B            1   // Device ID high byte
#define MHL_READ_DEVCAP_0x0C            1   // Device ID low byte
#define MHL_READ_DEVCAP_0x0D            1   // Scratchpad size
#define MHL_READ_DEVCAP_0x0E            1   // Interrupt /status size
#define MHL_READ_DEVCAP_0x0F            1   // Reserved
#define MHL_READ_XDEVCAP_0x80           1   // eCbus speed
#define MHL_READ_XDEVCAP_0x81           1   // TMDS speed
#define MHL_READ_XDEVCAP_0x82           1   // Device roles
#define MHL_READ_XDEVCAP_0x83           1   // Extended logical device map

#define MHL_READ_DEVCAP_L               (MHL_READ_DEVCAP_0x00 |(MHL_READ_DEVCAP_0x01 <<1) |(MHL_READ_DEVCAP_0x02 <<2) |(MHL_READ_DEVCAP_0x03 <<3)\
                                        |(MHL_READ_DEVCAP_0x04 <<4) |(MHL_READ_DEVCAP_0x05 <<5) |(MHL_READ_DEVCAP_0x06 <<6) |(MHL_READ_DEVCAP_0x07 <<7))
#define MHL_READ_DEVCAP_H               (MHL_READ_DEVCAP_0x08 |(MHL_READ_DEVCAP_0x09 <<1) |(MHL_READ_DEVCAP_0x0A <<2) |(MHL_READ_DEVCAP_0x0B <<3)\
                                        |(MHL_READ_DEVCAP_0x0C <<4) |(MHL_READ_DEVCAP_0x0D <<5) |(MHL_READ_DEVCAP_0x0E <<6) |(MHL_READ_DEVCAP_0x0F <<7))
#define MHL_READ_DEVCAP_MASK            (MHL_READ_DEVCAP_L |(MHL_READ_DEVCAP_H << 8))

#define MHL_READ_EXTEND_DEVCAP_MASK     (MHL_READ_XDEVCAP_0x80 |(MHL_READ_XDEVCAP_0x81 <<1) |(MHL_READ_XDEVCAP_0x82 <<2) |(MHL_READ_XDEVCAP_0x83 <<3))

#define MHL_DEVICE_CAPABILITY_SIZE      16
#define MHL_XDEVICE_CAPABILITY_SIZE     4

#define MHL_READ_DEVCAP_MASK_PORT0      MHL_READ_DEVCAP_MASK
#define MHL_READ_DEVCAP_MASK_PORT1      MHL_READ_DEVCAP_MASK
#define MHL_READ_DEVCAP_MASK_PORT2      MHL_READ_DEVCAP_MASK
#define MHL_READ_DEVCAP_MASK_PORT3      MHL_READ_DEVCAP_MASK
#define MHL_READ_DEVCAP_MASK_PORT4      MHL_READ_DEVCAP_MASK
#define MHL_READ_DEVCAP_MASK_PORT5      MHL_READ_DEVCAP_MASK

#define MHL_READ_EXTEND_DEVCAP_MASK0    MHL_READ_EXTEND_DEVCAP_MASK
#define MHL_READ_EXTEND_DEVCAP_MASK1    MHL_READ_EXTEND_DEVCAP_MASK
#define MHL_READ_EXTEND_DEVCAP_MASK2    MHL_READ_EXTEND_DEVCAP_MASK
#define MHL_READ_EXTEND_DEVCAP_MASK3    MHL_READ_EXTEND_DEVCAP_MASK
#define MHL_READ_EXTEND_DEVCAP_MASK4    MHL_READ_EXTEND_DEVCAP_MASK
#define MHL_READ_EXTEND_DEVCAP_MASK5    MHL_READ_EXTEND_DEVCAP_MASK

typedef enum
{
    MHL_SYSTEM_FORCE_DISPLAY_ENABLE = 0,
    MHL_SYSTEM_FORCE_DISPLAY_DISABLE,
    MHL_SYSTEM_FORCE_SEND_MSG_ENABLE,
    MHL_SYSTEM_FORCE_SEND_MSG_DISABLE,
}EN_MHL_SYSTEM_CONTROL;

enum MHL_MSC_MSG_KEY_EVENT_TYPE
{
    MHL_MSC_MSG_KEY_EVENT_NONE = 0,
    MHL_MSC_MSG_KEY_EVENT_UP,
    MHL_MSC_MSG_KEY_EVENT_DOWN,
    MHL_MSC_MSG_KEY_EVENT_MENU,
    MHL_MSC_MSG_KEY_EVENT_EXIT,
    MHL_MSC_MSG_KEY_EVENT_MUTE,
    MHL_MSC_MSG_KEY_EVENT_UNMUTE,
    MHL_MSC_MSG_KEY_EVENT_SWITCH_PORT,
};

#define GET_MHL_MSG_KEY_EVENT_PORT(a)       (ucMHLMSCMSGKeyEvent[a])
#define SET_MHL_MSG_KEY_EVENT_PORT(a,b)     (ucMHLMSCMSGKeyEvent[a] = b)

extern BYTE ucMHLMSCMSGKeyEvent[];

//-------------------------------------------------------------------------------------------------
//  Function Prototype
//-------------------------------------------------------------------------------------------------
void mapi_mhl_SetPowerStatus(BYTE ucPowerStatus);
void mapi_mhl_SetConfiguration(BYTE ucMHLEnableIndex, BYTE ucDisplayEnableIndex);
void mapi_mhl_SetSystemControlFlag(EN_MHL_SYSTEM_CONTROL ucControlType);
void mapi_mhl_LoadEDIDPort(BYTE ucPortSelect, BYTE **pComboEDID);
void mapi_mhl_Initial(BYTE **pComboEDID);
void mapi_mhl_TimerInterrupt(void);
void mapi_mhl_IsrHandler(InterruptNum eIntNum);
void mapi_mhl_Handler(void);
Bool mapi_mhl_SendRCPCommand(BYTE ucInputPort, BYTE ucKeyCode);
Bool mapi_mhl_SendRAPCommand(BYTE ucInputPort, BYTE ucKeyCode);
Bool mapi_mhl_SendUCPCommand(BYTE ucInputPort, BYTE ucKeyCode);
void mapi_mhl_SourceChange(void);
void mapi_mhl_PowerControl(BYTE ucState);

#if(MHL_GET_CABLE_DETECT_SUPPORT)
Bool mapi_mhl_GetCableDetectPortByInit(BYTE ucInputPort);
Bool mapi_mhl_GetCableDetectPort(BYTE ucInputPort);
#endif

Bool mapi_mhl_GetCbusConnectPort(BYTE ucInputPort);
BYTE mapi_mhl_GetMSGKeyEvent(BYTE ucInputPort);
BYTE mapi_mhl_SendEMSCDataString(BYTE ucPortSelect, BYTE ucLength, BYTE *ucSendData);
void mapi_mhl_CablePlugProcByInit(BYTE ucInputPort);
#endif //_MAPI_MHL_H_


