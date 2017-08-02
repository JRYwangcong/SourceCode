
#    -DENABLE_DMP=1                                                  \
#    for above slash

CC_MTOPTS += \
    -DOBA2=0                                                        \
    -DMSOS_TYPE_NOS                                                 \
    -DENABLE_DMP=0                                                  \
    -DENABLE_PIP=0                                                  \
    -DFOR_BENCH_CODE=0                                              \
    -DENCODE_AUDIO_AP=0                                             \
    -DENABLE_DEMO_FINE_TUNE=0                                       \
    -DENABLE_3D=0                                                   \
    -DENABLE_FORCE_MM_HD_FB=0                                       \
    -DENABLE_MIRROR=0                                               \
    -DModelName=MST9U_DEMO                                           \

#aeon1/aeonR2/aeon2
AEON_TYPE=aeonR2
CC_MTOPTS += -DMCU_AEON=1

##################################################################
# Bootloader
##################################################################
BL_SRC_FILE =

##################################################################
# Monitor Code
##################################################################
MONITOR_PATH               = $(ROOT)/monitor_ap
MONITOR_PATH_DRIVER        = $(MONITOR_PATH)/DRIVER
MONITOR_PATH_EXTDEVICE     = $(MONITOR_PATH)/EXTDEVICE
MONITOR_PATH_KERNAL_SCALER = $(MONITOR_PATH)/KERNEL/SCALER
MONITOR_PATH_KERNAL_SYSTEM = $(MONITOR_PATH)/KERNEL/SYSTEM
MONITOR_PATH_LIB           = $(MONITOR_PATH)/LIB
MONITOR_PATH_LIB_SRC                 = $(ROOT)/lib_src/monitor_lib
MONITOR_PATH_PANEL         = $(MONITOR_PATH)/PANEL
CUSTOM_PATH		   = $(ROOT)/../../Customer/SEC/MST9U/MST9U_AP/monitor_ap
MONITOR_PATH_CUSTOM_SEC_UI         = $(CUSTOM_PATH)/CUSTOM/SEC/UI
MONITOR_PATH_CUSTOM_SEC_SI         = $(CUSTOM_PATH)/CUSTOM/SEC/UI/SI
MONITOR_PATH_CUSTOM_SEC_ZUI        = $(CUSTOM_PATH)/CUSTOM/SEC/ZUI
MONITOR_PATH_CUSTOM_SEC_APP        = $(CUSTOM_PATH)/CUSTOM/SEC/APP
MONITOR_PATH_CUSTOM_SEC_USERDATA   = $(CUSTOM_PATH)/CUSTOM/SEC/USERDATA
MONITOR_PATH_CUSTOM_SEC_USERFUN    = $(CUSTOM_PATH)/CUSTOM/SEC/USERFUN
MONITOR_PATH_CUSTOM_SEC_USERFUN_SI = $(CUSTOM_PATH)/CUSTOM/SEC/USERFUN/SI


#        $(MONITOR_PATH_DRIVER)/mapi_mhl.c                             \
#        $(MONITOR_PATH_DRIVER)/mdrv_mhl.c                             \
#        $(MONITOR_PATH_DRIVER)/mhal_mhl.c                             \

MONITOR_FILES_DRIVER  += \
        $(MONITOR_PATH_DRIVER)/appmStar.c                             \
        $(MONITOR_PATH_DRIVER)/drv3DLUT.c                             \
        $(MONITOR_PATH_DRIVER)/drvDeltaE.c                             \
        $(MONITOR_PATH_DRIVER)/drvAdjust.c                            \
        $(MONITOR_PATH_DRIVER)/drvAudio.c                               \
        $(MONITOR_PATH_DRIVER)/drvDDC2Bi.c                            \
        $(MONITOR_PATH_DRIVER)/drvDPRxApp.c                           \
        $(MONITOR_PATH_DRIVER)/drvDPRxComboApp.c                           \
        $(MONITOR_PATH_DRIVER)/drvDPTxApp.c                           \
        $(MONITOR_PATH_DRIVER)/drvMcu.c                               \
        $(MONITOR_PATH_DRIVER)/mapi_combo.c                           \
        $(MONITOR_PATH_DRIVER)/drvMHL.c                               \
        $(MONITOR_PATH_DRIVER)/drvmsOVD.c                             \
        $(MONITOR_PATH_DRIVER)/drvmStar.c                             \
        $(MONITOR_PATH_DRIVER)/msScaler.c                             \
        $(MONITOR_PATH_DRIVER)/mapi_tmds.c                            \
        $(MONITOR_PATH_DRIVER)/drvDemura.c                            \
        $(MONITOR_PATH_DRIVER)/mapi_CEC.c                            \
        $(MONITOR_PATH_DRIVER)/usb_host/source/usb_host_p1/drvHost200.c                            \
        $(MONITOR_PATH_DRIVER)/usb_host/source/usb_host_p1/drvUsbHostLib.c                            \
        $(MONITOR_PATH_DRIVER)/usb_host/source/usb_host_p1/drvHostLib.c                            \
        $(MONITOR_PATH_DRIVER)/usb_host/source/usb_host_p1/drvMassStorage.c                            \
        $(MONITOR_PATH_DRIVER)/usb_host/source/usb_host_p1/drvScsi.c                            \
        $(MONITOR_PATH_DRIVER)/usb_host/source/usb_host_p1/drvUsbMain.c                            \
        #$(MONITOR_PATH_DRIVER)/usb_host/source/usb_hid_p1/drvhidcore.c                            \
        #$(MONITOR_PATH_DRIVER)/usb_host/source/usb_hid_p1/drvintpipe.c                            \

#        $(MONITOR_PATH_EXTDEVICE)/IT7230_I2C.c             \
#        $(MONITOR_PATH_EXTDEVICE)/IT7230_SettingTable.c    \
#        $(MONITOR_PATH_EXTDEVICE)/IT7230_TouchKey.c        \

MONITOR_FILES_EXTDEVICE  += \
        $(MONITOR_PATH_EXTDEVICE)/TouchKey.c               \
        $(MONITOR_PATH_EXTDEVICE)/TouchPanel.c             \
        $(MONITOR_PATH_EXTDEVICE)/USB.c                    \


#        $(MONITOR_PATH_KERNAL_SCALER)/msADC.c             \
#        $(MONITOR_PATH_KERNAL_SCALER)/msADC_TSUMXX.c      \
#        $(MONITOR_PATH_KERNAL_SCALER)/MsADC_TSUMXXTX.c    \
#        $(MONITOR_PATH_KERNAL_SCALER)/Mdrv_adctbl.c       \
#        $(MONITOR_PATH_KERNAL_SCALER)/msHDMIACE.c         \

MONITOR_FILES_KERNAL_SCALER  += \
        $(MONITOR_PATH_KERNAL_SCALER)/Adjust.c            \
        $(MONITOR_PATH_KERNAL_SCALER)/AutoFunc.c          \
        $(MONITOR_PATH_KERNAL_SCALER)/DDC.c               \
        $(MONITOR_PATH_KERNAL_SCALER)/detect.c            \
        $(MONITOR_PATH_KERNAL_SCALER)/drvADC.c            \
        $(MONITOR_PATH_KERNAL_SCALER)/drvGPIO.c           \
        $(MONITOR_PATH_KERNAL_SCALER)/drvOSD.c            \
        $(MONITOR_PATH_KERNAL_SCALER)/drvOSDblend.c       \
        $(MONITOR_PATH_KERNAL_SCALER)/LedControl.c        \
        $(MONITOR_PATH_KERNAL_SCALER)/ms2DTo3D.c          \
        $(MONITOR_PATH_KERNAL_SCALER)/msOVD.c             \
        $(MONITOR_PATH_KERNAL_SCALER)/mStar.c             \
        $(MONITOR_PATH_KERNAL_SCALER)/Ms_PM.c             \
        $(MONITOR_PATH_KERNAL_SCALER)/ms_rwreg.c          \
        $(MONITOR_PATH_KERNAL_SCALER)/UserDataSpace.c     \
        $(MONITOR_PATH_KERNAL_SCALER)/msScalerAP.c     	  \
        $(MONITOR_PATH_KERNAL_SCALER)/drvMWE.c     	  \

MONITOR_FILES_KERNAL_SYSTEM  += \
        $(MONITOR_PATH_KERNAL_SYSTEM)/COLOR_VERIFY.c    \
        $(MONITOR_PATH_KERNAL_SYSTEM)/Common.c          \
        $(MONITOR_PATH_KERNAL_SYSTEM)/Debug.c           \
        $(MONITOR_PATH_KERNAL_SYSTEM)/Global.c          \
        $(MONITOR_PATH_KERNAL_SYSTEM)/i2c.c             \
        $(MONITOR_PATH_KERNAL_SYSTEM)/hwi2c.c           \
        $(MONITOR_PATH_KERNAL_SYSTEM)/isr.c             \
        $(MONITOR_PATH_KERNAL_SYSTEM)/mcu.c             \
        $(MONITOR_PATH_KERNAL_SYSTEM)/misc.c            \
        $(MONITOR_PATH_KERNAL_SYSTEM)/mode.c            \
        $(MONITOR_PATH_KERNAL_SYSTEM)/MsFlash.c         \
        $(MONITOR_PATH_KERNAL_SYSTEM)/NVRam.c           \
        $(MONITOR_PATH_KERNAL_SYSTEM)/power.c           \
        $(MONITOR_PATH_KERNAL_SYSTEM)/UserPrefAPI.c     \
        $(MONITOR_PATH_KERNAL_SYSTEM)/apiDDC2Bi.c       \
        $(MONITOR_PATH_KERNAL_SYSTEM)/xmodem.c          \
		$(MONITOR_PATH_KERNAL_SYSTEM)/hwi2c_debug.c     \


MONITOR_FILES_LIB  += \
        $(MONITOR_PATH_LIB)/msComboRx.c                 \
        $(MONITOR_PATH_LIB)/MsDLC_Setting.c             \
        $(MONITOR_PATH_LIB)/msDPS_Setting.c             \
        $(MONITOR_PATH_LIB)/PQ/LIB/hal/mhal_pq.c        \
        $(MONITOR_PATH_LIB)/PQ/LIB/hal/QualityMap_Main.c\
        $(MONITOR_PATH_LIB)/PQ/LIB/hal/QualityMap_MainEx.c\
        $(MONITOR_PATH_LIB)/PQ/LIB/hal/QualityMap_Sub.c\
        $(MONITOR_PATH_LIB)/PQ/LIB/hal/QualityMap_SubEx.c\
        $(MONITOR_PATH_LIB)/PQ/LIB/hal/mhal_pq_adaptive.c\
        $(MONITOR_PATH_LIB)/PQ/LIB/drvPQ.c              \
        $(MONITOR_PATH_LIB)/PQ/msPQ.c                   \


MONITOR_FILES_LIB_FONT  += \
        $(MONITOR_PATH_LIB_SRC)/CompFont.c                  \
        $(MONITOR_PATH_LIB_SRC)/LoadPropFont1218.c          \
        $(MONITOR_PATH_LIB_SRC)/LoadPropFont1618.c          \

MONITOR_FILES_LIB_ACE  += \
        $(MONITOR_PATH_LIB_SRC)/msACE.c                     \

MONITOR_FILES_LIB_EREAD  += \
        $(MONITOR_PATH_LIB_SRC)/msEread.c                   \

MONITOR_FILES_LIB_DLC  += \
        $(MONITOR_PATH_LIB_SRC)/MsDLC.c                     \

MONITOR_FILES_LIB_DPS  += \
        $(MONITOR_PATH_LIB_SRC)/msDPS_Handler.c             \

MONITOR_FILES_LIB_DAISY_CHAIN  += \
        $(MONITOR_PATH_LIB_SRC)/msDaisyChain.c             \
		
MONITOR_LIB_FONT  = $(MONITOR_PATH_LIB)/LoadFont.lib
MONITOR_LIB_ACE   = $(MONITOR_PATH_LIB)/ACE.lib
MONITOR_LIB_EREAD = $(MONITOR_PATH_LIB)/Eread.lib
MONITOR_LIB_DLC   = $(MONITOR_PATH_LIB)/DLC.lib
MONITOR_LIB_DPS   = $(MONITOR_PATH_LIB)/DPS.lib
MONITOR_LIB_DAISY_CHAIN = $(MONITOR_PATH_LIB)/DaisyChain.lib

#       $(MONITOR_PATH_CUSTOM_SEC_UI)/DynamicLoadFont.c        \

MONITOR_FILES_CUSTOM_SEC_UI  += \
        $(MONITOR_PATH_CUSTOM_SEC_UI)/ColorPalette.c           \
        $(MONITOR_PATH_CUSTOM_SEC_UI)/MenuFont.c               \
        $(MONITOR_PATH_CUSTOM_SEC_UI)/menu.c                   \
        $(MONITOR_PATH_CUSTOM_SEC_UI)/MenuIcon.c               \
        $(MONITOR_PATH_CUSTOM_SEC_UI)/MenuPropFont.c           \
        $(MONITOR_PATH_CUSTOM_SEC_UI)/MenuPropFontAsia.c       \
        $(MONITOR_PATH_CUSTOM_SEC_UI)/MenuPropStr.c            \
        $(MONITOR_PATH_CUSTOM_SEC_UI)/msOSD.c                  \
        $(MONITOR_PATH_CUSTOM_SEC_UI)/msOSDDrawText.c          \
        $(MONITOR_PATH_CUSTOM_SEC_UI)/PropFontExtFunc.c        \
        $(MONITOR_PATH_CUSTOM_SEC_UI)/MenuACTFunc.c            \

MONITOR_FILES_CUSTOM_SEC_SI  += \
        $(MONITOR_PATH_CUSTOM_SEC_SI)/FactoryMenu.c         \
        $(MONITOR_PATH_CUSTOM_SEC_SI)/FMenuLoadFont.c       \
        $(MONITOR_PATH_CUSTOM_SEC_SI)/FMenuPropStr.c        \

MONITOR_FILES_CUSTOM_SEC_ZUI += \
        $(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_Init.c                              \
        $(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_Main.c                          \
        $(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTglobal.c                     \
        $(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTidle.c                       \
        $(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTMainMenu.c                   \
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTinputsource.c				\
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTSelectSource.c				\
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTMessageSetupGuide.c				\
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTMessageAutoShutdown.c				\
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTProgress.c				\
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTNotOptimum.c				\
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTMessagePopup.c 			\
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTMessageLogo.c 			\
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTRotateMessage.c			\
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTPopupAction.c			\
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTCheckSignal.c			\
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_SECprogress.c                    \
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_SECAutoCloseCount.c					 \
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_SECmarquee.c					 \
        $(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_GlobalFunction.c                    \
        $(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_LoadFont.c                          \
        $(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_LoadFontInit.c                      \
        $(MONITOR_PATH_CUSTOM_SEC_ZUI)/osdcomposer/OSDcp_global.c               \
        $(MONITOR_PATH_CUSTOM_SEC_ZUI)/osdcomposer/OSDcp_loadfont.c             \
        $(MONITOR_PATH_CUSTOM_SEC_ZUI)/osdcomposer/OSDcp_readbin.c              \
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_FactDraw.c				\
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTPopupSublist.c					 \
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTProgressPipContrast.c					 \
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTSelectSourcePortrait.c					 \
		$(MONITOR_PATH_CUSTOM_SEC_ZUI)/MApp_ZUI_ACTPBP4Window.c			\



MONITOR_FILES_CUSTOM_SEC_APP += \
        $(MONITOR_PATH_CUSTOM_SEC_APP)/appWindow.c                              \
        $(MONITOR_PATH_CUSTOM_SEC_APP)/appInput.c                               \
        $(MONITOR_PATH_CUSTOM_SEC_APP)/appSystem.c                              \
        $(MONITOR_PATH_CUSTOM_SEC_APP)/appAdjust.c                              \
        $(MONITOR_PATH_CUSTOM_SEC_APP)/main.c                                   \

MONITOR_FILES_CUSTOM_SEC_USERDATA += \
        $(MONITOR_PATH_CUSTOM_SEC_USERDATA)/UserPref.c                          \

MONITOR_FILES_CUSTOM_SEC_USERFUN += \
        $(MONITOR_PATH_CUSTOM_SEC_USERFUN)/MsIR.c                               \
        $(MONITOR_PATH_CUSTOM_SEC_USERFUN)/IrFunc.c	                            \
        $(MONITOR_PATH_CUSTOM_SEC_USERFUN)/Keypad.c                             \
        $(MONITOR_PATH_CUSTOM_SEC_USERFUN)/MenuFunc.c                           \
        $(MONITOR_PATH_CUSTOM_SEC_USERFUN)/DDC2Bi.c                             \
        $(MONITOR_PATH_CUSTOM_SEC_USERFUN)/MsSR.c                               \


MONITOR_FILES_CUSTOM_SEC_USERFUN_SI += \
        $(MONITOR_PATH_CUSTOM_SEC_USERFUN_SI)/FactoryAligment.c                 \
        $(MONITOR_PATH_CUSTOM_SEC_USERFUN_SI)/FMenuFunc.c                       \

MONITOR_FILES += \
        $(MONITOR_FILES_DRIVER)                     \
        $(MONITOR_FILES_EXTDEVICE)                  \
        $(MONITOR_FILES_KERNAL_SCALER)              \
        $(MONITOR_FILES_KERNAL_SYSTEM)              \
        $(MONITOR_FILES_LIB)                        \
        $(MONITOR_FILES_CUSTOM_SEC_UI)               \
        $(MONITOR_FILES_CUSTOM_SEC_SI)            \
        $(MONITOR_FILES_CUSTOM_SEC_APP)           \
        $(MONITOR_FILES_CUSTOM_SEC_USERDATA)      \
        $(MONITOR_FILES_CUSTOM_SEC_USERFUN)       \
        $(MONITOR_FILES_CUSTOM_SEC_USERFUN_SI)    \

ifeq ($(ZUI), 1)
MONITOR_FILES += \
        $(MONITOR_FILES_CUSTOM_SEC_ZUI)           \
        $(ZUI_FILES)
endif

MONITOR_FILES_INC += \
        -I$(MONITOR_PATH_DRIVER)/INC                                \
        -I$(MONITOR_PATH_EXTDEVICE)/INC                             \
        -I$(MONITOR_PATH_KERNAL_SCALER)/INC                         \
        -I$(MONITOR_PATH_KERNAL_SYSTEM)/INC                         \
        -I$(MONITOR_PATH_LIB)/INC                                   \
        -I$(MONITOR_PATH_LIB)/PQ/LIB/hal/INC                                     \
        -I$(MONITOR_PATH_LIB)/PQ/LIB/INC                                         \
        -I$(MONITOR_PATH_PANEL)/INC                                 \
        -I$(MONITOR_PATH_CUSTOM_SEC_UI)/INC                                       \
        -I$(CUSTOM_PATH)/CUSTOM/SEC/MODEL                                     \
        -I$(MONITOR_PATH_CUSTOM_SEC_SI)/INC                                    \
        -I$(MONITOR_PATH_CUSTOM_SEC_APP)/INC                                   \
        -I$(CUSTOM_PATH)/CUSTOM/SEC/BININFO                        \
        -I$(MONITOR_PATH_CUSTOM_SEC_USERDATA)/INC			       \
        -I$(MONITOR_PATH_CUSTOM_SEC_USERFUN)/INC                               \
        -I$(MONITOR_PATH_CUSTOM_SEC_USERFUN_SI)/INC                            \
        -I$(MONITOR_PATH_DRIVER)/usb_host/source                            \
        -I$(MONITOR_PATH_DRIVER)/usb_host                            \

ifeq ($(ZUI), 1)
MONITOR_FILES_INC += \
        -I$(MONITOR_PATH_CUSTOM_SEC_ZUI)/include                               \
        -I$(MONITOR_PATH_CUSTOM_SEC_ZUI)/osdcomposer/include                   \
        -I$(MONITOR_PATH_CUSTOM_SEC_ZUI)/res1366x768x565/Osdcomposer/Include
endif


# Source files
SRC_FILE += \
        $(MONITOR_FILES)      \


# Add "Header (include) file" directories here ...
INC_DIR  += \
        $(MONITOR_FILES_INC)      \


##################################################################
# Driver / API
##################################################################
#        $(ROOT)/core/driver/sys/MST9U/drvI2C.c                        \
#    for above slash
SYS_INIT_FILES += \
        $(ROOT)/core/driver/sys/MST9U/SysInit.c                       \

DRV_FILES += \
        $(ROOT)/core/driver/sys/MST9U/drvUartDebug.c                  \
        $(ROOT)/core/driver/sys/MST9U/drvISR.c                        \
        $(ROOT)/core/driver/sys/MST9U/drvPadConf.c                    \
        $(ROOT)/core/driver/sys/MST9U/drvInit.c                       \


#        $(ROOT)/core/middleware/mwutil/mw_debug.c                   \
#        $(ROOT)/core/util/util_minidump.c                           \
#        $(ROOT)/core/util/mstar_debug.c                             \
#        $(ROOT)/core/util/util_checkversion.c                       \
#    for above slash
UTIL_FILES += \
        $(ROOT)/core/util/util_symbol.c                             \
        $(ROOT)/core/util/uartdebug.c                               \


SRC_FILE += \
        $(SYS_INIT_FILES)                                           \
        $(DRV_FILES)                                                \
        $(UTIL_FILES)                                               \
        $(API_FILES)                                                \


#        -I$(ROOT)/core/middleware/mwutil/include                    \
#    for above slash
INC_DIR  += \
        -I$(ROOT)/project/image                                     \
        -I$(ROOT)/project/mmap                                      \
        -I$(ROOT)/core/driver/sys/MST9U/include                       \
        -I$(ROOT)/core/util/include                                 \
        -I$(ROOT)/core/api/include                                  \
        -I$(ROOT)/core/api/utl                                      \
        -I$(ROOT)/include/std                                       \

LINT_INC  += \
        $(ROOT)/project/image                                       \
        $(ROOT)/core/util/include                                   \
        $(ROOT)/core/api/include                                    \

##################################################################
# MONITOR-AP
##################################################################
MONAP = $(ROOT)/monitor-ap

#APP_FILES += \
#        $(MONAP)/main/app/MApp_Main.c                                 \


PROJS_FILES += \
        $(ROOT)/project/image/default/InfoBlock.c                      \
        $(ROOT)/project/image/imginfo.c                             \



# Source files
SRC_FILE += \
        $(APP_FILES)                                                \
        $(PROJS_FILES)                                              \

# Add "Header (include) file" directories here ...
INC_DIR  += \
        -I$(ROOT)/project/boarddef                                  \
        -I$(ROOT)/project/swdef                                     \
        -I$(ROOT)/project/image/default                             \
        -I$(ROOT)/project/image/bininfo/MST9U                        \
        -I$(ROOT)/project/mmap                                      \
        -I$(ROOT)/device/include                                    \


LINT_INC  += \
        $(ROOT)/project/boarddef                                    \
        $(ROOT)/project/swdef                                       \
        $(ROOT)/project/image/default                               \
        $(ROOT)/project/image/bininfo/MST9U                          \
        $(ROOT)/project/mmap                                        \
        $(ROOT)/device/include                                      \

LIB_PRANA_PATH = $(ROOT)/core/lib/MST9U
LIB_PRANA      = libprana_MST9U.a

LDLIB += -Wl,--whole-archive $(LIB_PRANA_PATH)/$(LIB_PRANA) -Wl,--no-whole-archive

DRV_LIB_PATH   = $(ROOT)/core/driver
API_LIB_PATH   = $(ROOT)/core/api
DRV_BSP_PATH   = $(ROOT)/core/drv_bsp/MST9U_nos_r2/lib
MIDDLE_LIB_PATH   = $(ROOT)/core/middleware


ifeq ($(BUILD_APLIB), y)
MONITOR_FILES += \
        $(MONITOR_FILES_LIB_FONT)                   \
        $(MONITOR_FILES_LIB_ACE)                    \
        $(MONITOR_FILES_LIB_EREAD)                  \
        $(MONITOR_FILES_LIB_DLC)                    \
        $(MONITOR_FILES_LIB_DPS)                    \
		$(MONITOR_FILES_LIB_DAISY_CHAIN) 
else
MONITOR_AP_LIB += \
        $(MONITOR_LIB_FONT)                         \
        $(MONITOR_LIB_ACE)                          \
        $(MONITOR_LIB_EREAD)                        \
        $(MONITOR_LIB_DLC)                          \
        $(MONITOR_LIB_DPS)                          \
		$(MONITOR_LIB_DAISY_CHAIN)
endif


MONITOR_LIB += \
    $(DRV_BSP_PATH)/libdrvBDMA.a                                        \
    $(DRV_BSP_PATH)/libdrvSERFLASH.a                                    \
    $(DRV_BSP_PATH)/libdrvMMIO.a                                        \
    $(DRV_BSP_PATH)/libdrvUART.a                                        \
    $(DRV_BSP_PATH)/libdrvMIU.a                                         \
    $(DRV_BSP_PATH)/libdrvWDT.a                                         \
    $(DRV_BSP_PATH)/libdrvSEM.a                                         \
    $(DRV_BSP_PATH)/libapiGFX.a                                         \
    $(DRV_BSP_PATH)/libapiGOP.a                                         \
    $(API_LIB_PATH)/mvf/libmvf_aeonR2.a                                 \

ifeq ($(VERIFY_GE), y)
	MONITOR_LIB += $(DRV_BSP_PATH)/libapiVerGE.a
endif

ifeq ($(BUILD_LIB),y)
LIB_SRC_ROOT = $(ROOT)/lib_src

LIB_PRANA_ROOT = $(LIB_SRC_ROOT)/platform/prana2

LIB_MXLIB = mxlib
LIB_MXLIB_ROOT = $(LIB_SRC_ROOT)/project/MST9U_nos_r2
endif

#Middleware libraries

BIN_INFO = $(ROOT)/$(CUSTOM_PATH)/CUSTOM/SEC/BININFO/BinInfo.h

####  REBUILD_FILES=$(MONAP)/main/ui2_M10/MApp_ZUI_ACTmenufunc.c

include project/build/FILES_COMMON.mk

MONITOR_LIB += \
    $(COMMON_MONITOR_LIB)
