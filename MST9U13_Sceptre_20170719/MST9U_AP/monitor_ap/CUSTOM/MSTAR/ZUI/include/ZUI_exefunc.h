////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////
// Window Exe Action Enum

typedef enum
{
    EN_EXE_ACTION_NULL,                         //=0

    //general actions:
    EN_EXE_CLOSE_CURRENT_OSD,                   ///< close current osd page
    EN_EXE_POWEROFF,                            ///< Power off
    EN_EXE_NAV_KEY_UP,                          ///< navigation go up
    EN_EXE_NAV_KEY_DOWN,                        ///< navigation go down
    EN_EXE_NAV_KEY_LEFT,                        ///< navigation go left
    EN_EXE_NAV_KEY_RIGHT,                       ///< navigation go right
    EN_EXE_NOTIFY_PARENT_KEY_SELECT,
    EN_EXE_NOTIFY_PARENT_KEY_UP,                ///< notify parent: up
    EN_EXE_NOTIFY_PARENT_KEY_DOWN,              ///< notify parent: down
    EN_EXE_NOTIFY_PARENT_KEY_LEFT,              ///< notify parent: left
    EN_EXE_NOTIFY_PARENT_KEY_RIGHT,             ///< notify parent: right



    //transition actions:
    EN_EXE_GOTO_EMPTYOSD,                       ///< Go to empty OSD
    EN_EXE_GOTO_MAINMENU,                       ///< Go to Main Menu Page
    EN_EXE_GOTO_MAIN_DVBTYPE,               ///< Go to Main Menu Channel page
    EN_EXE_GOTO_AUTO_TUNING,                    ///< Go to auto tuning
    EN_EXE_GOTO_ATV_TUNING,                     ///< Go to ATV man tuning
    EN_EXE_GOTO_DTV_TUNING,                     ///< Go to DTV man tuning
    EN_EXE_GOTO_CADTV_TUNING,                     ///< Go to CADTV man tuning
    EN_EXE_GOTO_PROGRAMEDIT,                    ///< go to program edit
    EN_EXE_GOTO_BLOCK_PROGRAM,                  ///< go to block program
    EN_EXE_GOTO_AUDIO_LANG,                     ///< go to audio language
    EN_EXE_GOTO_TTX,                            ///< go to TTX
    EN_EXE_GOTO_FACTORY_MENU   ,                ///< go to factory menu
    EN_EXE_GOTO_EXPERT_MENU    ,                ///< go to expert menu
    EN_EXE_GOTO_MEDIA_PLAYER   ,                ///< go to media player
    EN_EXE_GOTO_BT,                             ///< go to Bit torrent download
    EN_EXE_GOTO_KTV,                        ///< go to KTV
    EN_EXE_GOTO_CHANNEL_SIGNAL_INFORMAT,        ///< go to channel signal informat page
    EN_EXE_UPDATE_SIGNAL_INFORMAT,              ///< update channel signal informat
    EN_EXE_GOTO_CI_INFORMATION,                 ///< go to CI information
    EN_EXE_GOTO_CI_POLLING,                 ///< go to CI polling state
    EN_EXE_GOTO_CI_CLEAN_UP,                    ///< go to CI clean up state
    EN_EXE_GOTO_CHANNEL_LIST,                   ///< go to channel list
    EN_EXE_GOTO_FAVORITE_LIST,                  ///< go to favorite list
    EN_EXE_GOTO_PICTURE_MODE_PAGE,              ///< go to picture mode page
    EN_EXE_RETURN_PICTURE_MODE_PAGE,            ///< return to picture mode page
    EN_EXE_RETURN_PICTURE_COLOR_PAGE,           ///< return to picture color page
    EN_EXE_GOTO_PICTURE_COLOR_PAGE,             ///< go to picture color page
    EN_EXE_GOTO_SOUND_MODE_PAGE,                ///< go to sound mode page
    EN_EXE_GOTO_SOUND_BALANCE_PAGE,          	///< go to sound balance page
    EN_EXE_GOTO_SOUND_SPDIF_AUDIODELAY_PAGE, 	///< go to sound audiodelay page
    EN_EXE_GOTO_SOUND_EQ_PAGE,                  ///< go to sound EQ page
    EN_EXE_GOTO_SOUND_SWITCH_PAGE,              ///< go to sound switch page

    //increase/decrease options:
    //for DVB type select page
    EN_EXE_GOTO_DVB_SELECT,
    EN_EXE_DVB_TYPE_SELECT,
    EN_EXE_DEC_DVBC_SCAN_TYPE,                  ///< decrease DVBC Scan TYPE (Full /NetworkID)
    EN_EXE_INC_DVBC_SCAN_TYPE,                  ///< increase DVBC Scan TYPE (Full /NetworkID)
    EN_EXE_CHANGE_DVBC_FREQ_TYPE,           ///change DVBC scan Frequency auto or manual
    EN_EXE_CHANGE_DVBC_NID_TYPE,           ///change DVBC scan NetworkID auto or manual
    EN_EXE_CHANGE_DVBC_SYMBOL_TYPE,           ///change DVBC scan symbol rate auto or manual

    //increase/decrease options:
    //for picture page
    EN_EXE_DEC_CONTRAST,                        ///< decrease contrast
    EN_EXE_INC_CONTRAST,                        ///< increase contrast
    EN_EXE_DEC_SATURATION,                      ///< decrease saturation
    EN_EXE_INC_SATURATION,                      ///< increase saturation
    EN_EXE_DEC_BRIGHTNESS,                      ///< decrease brightness
    EN_EXE_INC_BRIGHTNESS,                      ///< increase brightness
    EN_EXE_DEC_SHARPNESS,                       ///< decrease sharpness
    EN_EXE_INC_SHARPNESS,                       ///< increase sharpness
    EN_EXE_DEC_TINT,                            ///< decrease tint
    EN_EXE_INC_TINT,                            ///< increase tint
    EN_EXE_DEC_PICTURE_MODE,                    ///< decrease picture mode
    EN_EXE_INC_PICTURE_MODE,                    ///< increase picture mode
    EN_EXE_DEC_COLOR_TEMP,                      ///< decrease color temp
    EN_EXE_INC_COLOR_TEMP,                      ///< increase color temp
    EN_EXE_DEC_COLOR_RED,                       ///< decrease color temp: RED
    EN_EXE_INC_COLOR_RED,                       ///< increase color temp: RED
    EN_EXE_DEC_COLOR_GREEN,                     ///< decrease color temp: GREEN
    EN_EXE_INC_COLOR_GREEN,                     ///< increase color temp: GREEN
    EN_EXE_DEC_COLOR_BLUE,                      ///< decrease color temp: BLUE
    EN_EXE_INC_COLOR_BLUE,                      ///< increase color temp: BLUE
    EN_EXE_SINGLELIST_UP,                       ///< raise up in single list
    EN_EXE_SINGLELIST_DOWN,                     ///< fall down in single list
    EN_EXE_OPTIONLIST_LEFT,                       ///< raise up in option list
    EN_EXE_OPTIONLIST_RIGHT,                     ///< fall down in option list
    EN_EXE_DEC_ASPECT_RATIO,                    ///< decrease aspect ratio
    EN_EXE_INC_ASPECT_RATIO,                    ///< increase aspect ratio

    //for PIP page
    EN_EXE_GOTO_PIP,                            ///< goto pip adjust page
    EN_EXE_CLOSE_PIP_PAGE,                      ///< exit pip adjust page
    EN_EXE_DEC_PIPMODE,                         ///< decrease PIP mode
    EN_EXE_INC_PIPMODE,                         ///< increase PIP mode
    EN_EXE_DEC_SUBSRC,                          ///< decrease sub input source
    EN_EXE_INC_SUBSRC,                          ///< increase sub input source
    EN_EXE_DEC_PIP_SIZE,                        ///< decrease sub window size
    EN_EXE_INC_PIP_SIZE,                        ///< increase sub window size
    EN_EXE_DEC_PIP_POSITION,                    ///< decrease sub window position
    EN_EXE_INC_PIP_POSITION,                    ///< increase sub window position
    EN_EXE_DEC_PIP_BORDER,                      ///< decrease sub window border
    EN_EXE_INC_PIP_BORDER,                      ///< increase sub window border
    EN_EXE_DEC_PIP_SOUND_SRC,                   ///< decrease sub window border
    EN_EXE_INC_PIP_SOUND_SRC,                   ///< increase sub window border
    EN_EXE_PIP_SWAP,                            ///< execute main/sub input_source swap

    //for sound page
    EN_EXE_DEC_SOUND_120_HZ,                    ///< decrease sound 120hz
    EN_EXE_INC_SOUND_120_HZ,                    ///< increase sound 120hz
    EN_EXE_DEC_SOUND_200_HZ,                    ///< decrease sound 200hz
    EN_EXE_INC_SOUND_200_HZ,                    ///< increase sound 200hz
    EN_EXE_DEC_SOUND_500_HZ,                    ///< decrease sound 500hz
    EN_EXE_INC_SOUND_500_HZ,                    ///< increase sound 500hz
    EN_EXE_DEC_SOUND_1_2_KHZ,                    ///< decrease sound 1.2khz
    EN_EXE_INC_SOUND_1_2_KHZ,                    ///< increase sound 1.2khz
    EN_EXE_DEC_SOUND_3_KHZ,                    ///< decrease sound 3khz
    EN_EXE_INC_SOUND_3_KHZ,                    ///< increase sound 3khz
    EN_EXE_DEC_SOUND_7_5_KHZ,                    ///< decrease sound 7.5khz
    EN_EXE_INC_SOUND_7_5_KHZ,                    ///< increase sound 7.5khz
    EN_EXE_DEC_SOUND_12_KHZ,                    ///< decrease sound 12khz
    EN_EXE_INC_SOUND_12_KHZ,                    ///< increase sound 12khz
    EN_EXE_DEC_SOUND_MODE,                    ///< decrease sound mode
    EN_EXE_INC_SOUND_MODE,                    ///< increase sound mode
    EN_EXE_DEC_TREBLE,                    ///< decrease sound treble
    EN_EXE_INC_TREBLE,                    ///< increase sound treble
    EN_EXE_DEC_BASS,                    ///< decrease sound bass
    EN_EXE_INC_BASS,                    ///< increase sound bass
    EN_EXE_DEC_BALANCE,                    ///< decrease sound balance
    EN_EXE_INC_BALANCE,                    ///< increase sound balance
    EN_EXE_DEC_SPDIF_AUDIODELAY,			///< decrease audio delay
    EN_EXE_INC_SPDIF_AUDIODELAY,			///< increase audio delay
    EN_EXE_DEC_SURROUND_SOUND,                    ///< decrease surround sound
    EN_EXE_INC_SURROUND_SOUND,                    ///< increase surround sound
    EN_EXE_DEC_SOUND_AD_FADE,                    ///< decrease sound ad fade
    EN_EXE_INC_SOUND_AD_FADE,                    ///< increase sound ad fade
    EN_EXE_DEC_SOUND_AD_PAN,                    ///< decrease sound ad pan
    EN_EXE_INC_SOUND_AD_PAN,                    ///< increase sound ad pan
    //for time page
    EN_EXE_DEC_SLEEP_TIMER,                     ///< decrease sleep timer
    EN_EXE_INC_SLEEP_TIMER,                     ///< increase sleep timer
    EN_EXE_DEC_TIMEZONE,                        ///< decrease timezone setting
    EN_EXE_INC_TIMEZONE,                        ///< increase timezone setting
    //for option page
    EN_EXE_AUDIOLANG_SET,                                   ///< switch primary and secondary
    EN_EXE_SUBLANG_SET,                                   ///< switch primary and secondary
    EN_EXE_DEC_PARENTAL_GUIDANCE,                    ///< decrease parental guidance
    EN_EXE_INC_PARENTAL_GUIDANCE,                    ///< increase parental guidance
    //for screen page
    EN_EXE_DEC_PCMODE_HPOS,                     ///< decrease pcmode h-pos
    EN_EXE_INC_PCMODE_HPOS,                     ///< increase pcmode h-pos
    EN_EXE_DEC_PCMODE_VPOS,                     ///< decrease pcmode V-pos
    EN_EXE_INC_PCMODE_VPOS,                     ///< increase pcmode V-pos
    EN_EXE_DEC_PCMODE_SIZE,                     ///< decrease pcmode size
    EN_EXE_INC_PCMODE_SIZE,                     ///< increase pcmode size
    EN_EXE_DEC_PCMODE_PHASE,                     ///< decrease pcmode phase
    EN_EXE_INC_PCMODE_PHASE,                     ///< increase pcmode phase
    //for set clock dialog
    EN_EXE_DEC_SET_CLOCK_DAY,                       ///< decrease set clock day
    EN_EXE_INC_SET_CLOCK_DAY,                       ///< increase set clock day
    EN_EXE_TENKEY_SET_CLOCK_DAY,                    // set day by ten key
    EN_EXE_DEC_SET_CLOCK_MONTH,                     ///< decrease set clock month
    EN_EXE_INC_SET_CLOCK_MONTH,                     ///< increase set clock month
    EN_EXE_TENKEY_SET_CLOCK_MONTH,                  // set month by ten key
    EN_EXE_DEC_SET_CLOCK_YEAR,                      ///< decrease set clock year
    EN_EXE_INC_SET_CLOCK_YEAR,                      ///< increase set clock year
    EN_EXE_TENKEY_SET_CLOCK_YEAR,                   // set year by ten key
    EN_EXE_DEC_SET_CLOCK_HOUR,                      ///< decrease set clock hour
    EN_EXE_INC_SET_CLOCK_HOUR,                      ///< increase set clock hour
    EN_EXE_TENKEY_SET_CLOCK_HOUR,                   // set hour by ten key
    EN_EXE_DEC_SET_CLOCK_MIN,                       ///< decrease set clock minute
    EN_EXE_INC_SET_CLOCK_MIN,                       ///< increase set clock minute
    EN_EXE_TENKEY_SET_CLOCK_MIN,                    // set min by ten key
    //for set off time dialog
    EN_EXE_DEC_SET_OFFTIME_REPEAT,                    ///< decrease set offtime repeat style
    EN_EXE_INC_SET_OFFTIME_REPEAT,                    ///< increase set offtime repeat style
    EN_EXE_DEC_SET_OFFTIME_HOUR,                      ///< decrease set offtime hour
    EN_EXE_INC_SET_OFFTIME_HOUR,                      ///< increase set offtime hour
    EN_EXE_TENKEY_SET_OFFTIME_HOUR,                   // set off time hour by ten key
    EN_EXE_DEC_SET_OFFTIME_MIN,                       ///< decrease set offtime minute
    EN_EXE_INC_SET_OFFTIME_MIN,                       ///< increase set offtime minut
    EN_EXE_TENKEY_SET_OFFTIME_MIN,                    //set off time min by ten key
    //for set on time dialog
    EN_EXE_DEC_SET_ONTIME_REPEAT,                    ///< decrease set ontime repeat style
    EN_EXE_INC_SET_ONTIME_REPEAT,                    ///< increase set ontime repeat style
    EN_EXE_DEC_SET_ONTIME_HOUR,                      ///< decrease set ontime hour
    EN_EXE_INC_SET_ONTIME_HOUR,                      ///< increase set ontime hour
    EN_EXE_TENKEY_SET_ONTIME_HOUR,                   // set on time hour by ten key
    EN_EXE_DEC_SET_ONTIME_MIN,                       ///< decrease set ontime minute
    EN_EXE_INC_SET_ONTIME_MIN,                       ///< increase set ontime minute
    EN_EXE_TENKEY_SET_ONTIME_MIN,                    // set on time min by ten key
    EN_EXE_DEC_SET_ONTIME_SOURCE,                    ///< decrease set ontime source
    EN_EXE_INC_SET_ONTIME_SOURCE,                    ///< increase set ontime source
    EN_EXE_DEC_SET_ONTIME_CHANNEL,                   ///< decrease set ontime channel
    EN_EXE_INC_SET_ONTIME_CHANNEL,                   ///< increase set ontime channel
    EN_EXE_DEC_SET_ONTIME_VOLUME,                    ///< decrease set ontime volume
    EN_EXE_INC_SET_ONTIME_VOLUME,                    ///< increase set ontime volume
    EN_EXE_TENKEY_SET_ONTIME_VOLUME,                 //  set on time volumn by ten key
    //for install guide, tuning confirm dialog
    EN_EXE_DEC_TEMP_COUNTRY,                    ///< decrease TEMP country
    EN_EXE_INC_TEMP_COUNTRY,                    ///< increase TEMP country
    EN_EXE_DEC_TUNE_TYPE,                  ///< decrease TEMP TUNE TYPE (DTV/ATV/DTV+ATV)
    EN_EXE_INC_TUNE_TYPE,                  ///< increase TEMP TUNE TYPE (DTV/ATV/DTV+ATV)
    //for hotkeys
    EN_EXE_DEC_AUDIO_VOLUME,                    ///< decrease audio volume
    EN_EXE_INC_AUDIO_VOLUME,                    ///< increase audio volume
    EN_EXE_DEC_SLEEP_HOTKEY_OPTION,             ///< decrease option: sleep hotkey
    EN_EXE_INC_SLEEP_HOTKEY_OPTION,             ///< increase option: sleep hotkey
    EN_EXE_DEC_PICTURE_HOTKEY_OPTION,           ///< decrease option: picture hotkey
    EN_EXE_INC_PICTURE_HOTKEY_OPTION,           ///< increase option: picture hotkey
    EN_EXE_DEC_PIP_HOTKEY_OPTION,               ///< decrease option: pip hotkey
    EN_EXE_INC_PIP_HOTKEY_OPTION,               ///< increase option: pip hotkey
    EN_EXE_DEC_AUDIO_HOTKEY_OPTION,             ///< decrease option: audio hotkey
    EN_EXE_INC_AUDIO_HOTKEY_OPTION,             ///< increase option: audio hotkey
    EN_EXE_DEC_SCALE_HOTKEY_OPTION,             ///< decrease option: scale hotkey
    EN_EXE_INC_SCALE_HOTKEY_OPTION,             ///< increase option: scale hotkey
    EN_EXE_DEC_ATV_MTS_HOTKEY_OPTION,           ///< decrease option: ATV MTS hotkey
    EN_EXE_INC_ATV_MTS_HOTKEY_OPTION,           ///< increase option: ATV MTS hotkey
    //for atv manual tuning
    EN_EXE_DEC_STORAGE_TO_OPTION,               ///< decrease storage to vaule
    EN_EXE_INC_STORAGE_TO_OPTION,               ///< increase storage to vaule
    EN_EXE_DEC_SYSTEM_OPTION,                   ///< decrease system vaule
    EN_EXE_INC_SYSTEM_OPTION,                   ///< increase system vaule
    EN_EXE_DEC_CURRENT_CH_OPTION,               ///< decrease current ch. vaule
    EN_EXE_INC_CURRENT_CH_OPTION,               ///< increase current ch. vaule
    // for noise reduction
    EN_EXE_DEC_NOISE_REDUCTION,                 ///< On Off switch noise reduction
    EN_EXE_INC_NOISE_REDUCTION,                 ///< On Off switch noise reduction
    // for OSD animation
    EN_EXE_DEC_OSD_EFFECT_MODE,                 ///< On off switch OSD animation
    EN_EXE_INC_OSD_EFFECT_MODE,                 ///< On off switch OSD animation

    // ofr caption
    EN_EXE_DEC_OPTION_CAPTION,                   ///< decrease option vaule
    EN_EXE_INC_OPTION_CAPTION,                  ///< increase option vaule

    EN_EXE_SW_USB_UPDATE,
    //on/off switch
    EN_EXE_ONOFF_SWUPDATE,                      ///< On Off switch software update
    EN_EXE_ONOFF_5VANTENNA,                     ///< On Off switch 5V antenna power
    EN_EXE_ONOFF_AUTO_VOLUME,                   ///< On Off switch auto volume
    EN_EXE_ONOFF_AUTO_SLEEP,                    ///< On Off switch auto sleep
    EN_EXE_ONOFF_HARD_HEARING,                  ///< On Off switch hard hearing
    EN_EXE_ONOFF_LOCK_SYSTEM,                   ///< On Off switch lock system
    EN_EXE_ONOFF_SOUND_AD,                      ///< On off switch sound ad
    EN_EXE_ONOFF_FREEZE_HOTKEY_OPTION,          ///< On off picture freeze
    EN_EXE_ONOFF_SKIP_OPTION,                   ///< On off skip
    EN_EXE_ONOFF_RSS,                           ///< On Off switch RSS
    EN_EXE_ONOFF_EXTENSION,                     ///< On Off switch EXTENSION
    EN_EXE_DEC_NET_CONFIG,                    ///< On Off switch network configuration
    EN_EXE_INC_NET_CONFIG,
    EN_EXE_SPDIF_MODE_PCM_AUTO,                  ///< switch PCM and AUTO mode
    EN_EXE_COLOR_RANGE_SETTING,                 ///< switch color range 0~255 and 16~235

    //repaint options:
    EN_EXE_REPAINT_ALL,                         ///< invalidate/repaint all windows
    EN_EXE_REPAINT_AUTOTUNING_PROGRESS,         ///< invalidate/repaint auto tuning progress
    EN_EXE_REPAINT_CIMMI_PAGE,                  ///< invalidate/repaint CI/MMI

    //hotkey:
    EN_EXE_SHOW_SLEEP_HOTKEY,                   ///< show sleep hotkey
    EN_EXE_SHOW_PICTURE_HOTKEY,                 ///< show picture hotkey
    EN_EXE_SHOW_PIP_HOTKEY,                     ///< show pip hotkey
    EN_EXE_SHOW_AUDIO_HOTKEY,                   ///< show audio hotkey
    EN_EXE_SHOW_SCALE_HOTKEY,                   ///< show scale hotkey
    EN_EXE_SHOW_ATV_MTS_HOTKEY,                 ///< show ATV MTS hotkey
    EN_EXE_SHOW_FREEZE_HOTKEY,                  ///< show freeze hotkey

    //channel info (channel banner):
    EN_EXE_HIDE_CH_INFO,                        ///< hide CH INFO
    EN_EXE_REPAINT_CH_INFO,                     ///< invalidate/repaint CH INFO
    EN_EXE_REPAINT_CH_INFO_SCROLL,              ///< scroll event info descriptor
    EN_EXE_SHOW_BRIEF_CH_INFO,                  ///< show brief CH INFO
    EN_EXE_SHOW_EXTENDED_CH_INFO,               ///< show ext CH INFO
    //EN_EXE_SHOW_NEXT_EVENT_CH_INFO,           ///< show next event in CH INFO
    //EN_EXE_SHOW_NOW_EVENT_CH_INFO,            ///< show now event in CH INFO
    EN_EXE_HIDE_CH_BANNER,                      ///< hide banner
    EN_EXE_REPAINT_CH_BANNER,                   ///< invalidate/repaint channel banner (top part)
    EN_EXE_SHOW_TV_BANNER,                      ///< show TV channel banner
    //EN_EXE_SHOW_ATV_BANNER,                   ///< show ATV banner
    //EN_EXE_SHOW_DTV_BANNER,                   ///< show DTV banner
    EN_EXE_SHOW_SOURCE_BANNER,                  ///< show source banner

    //message box: (NOTE: ORDER is important! need to be sync with enum _MSGBOX_MODE)
    EN_EXE_SHOW_FUNC_NOT_AVAIL_MSGBOX    ,      ///< show function not availiable message box
    EN_EXE_SHOW_MHEG5_SUBTITLE_MSGBOX    ,      ///< show MHEG5 subtitle co-exist message box
    EN_EXE_SHOW_LOADING_MHEG5_MSGBOX    ,       ///< show loading MHEG5 message box
    EN_EXE_SHOW_AUTO_ADJUSTING_MSGBOX    ,      ///< show auto adjusting message box
    EN_EXE_SHOW_NO_TELETEXT_MSGBOX,             ///< show no teletext message box
    EN_EXE_SHOW_NO_TTX_SUBTITLE_MSGBOX,             ///< show no teletext subtitle message box
    EN_EXE_SHOW_KEY_LOCK_MSGBOX    ,            ///< show key lock message box
    EN_EXE_SHOW_CEC_DEVICE_MSGBOX,              ///< show HDMI CEC device message box
    EN_EXE_SHOW_POWER_OFF_COUNTDOWN_MSG_BOX, ///< show power off countdown message box
    EN_EXE_SHOW_PASSWORD_INPUT_MSGBOX,      ///< show password input message box

    EN_EXE_SHOW_NETWORK_CHANGE_MSGBOX,
    EN_EXE_SHOW_NEW_MULTIPLEX_MIGHT_AVAILABLE_MSGBOX,
    EN_EXE_SHOW_MULTIPLEX_IS_REMOVED_MSGBOX,
    EN_EXE_SHOW_FREQUENCY_CHANGE_MSGBOX,
    EN_EXE_SHOW_LOSS_OF_SIGNAL_MSGBOX,
    EN_EXE_SHOW_SCANNING_MSGBOX,
    //message box buttons
    EN_EXE_MESSAGE_BOX_BTN_SELECT_YES,
    EN_EXE_MESSAGE_BOX_BTN_SELECT_NO,
    EN_EXE_MESSAGE_BOX_BTN_SELECT_OK,
    EN_EXE_CLEAR_CHLIST_BTN_YES,
    EN_EXE_CLEAR_CHLIST_BTN_NO,

    //message box for CI/MMI: (NOTE: ORDER is important! need to be sync with enum _MSGBOX_MODE)
    EN_EXE_SHOW_CI_DETECTED_MSGBOX,                     ///< show CI detected
    EN_EXE_SHOW_CI_REMOVED_MSGBOX,                     ///< show CI removed
    EN_EXE_SHOW_CI_LOAD_INFO_MSGBOX,                     ///< show CI load info
    EN_EXE_SHOW_CI_NO_MODULE_MSGBOX,                     ///< show CI no module
    EN_EXE_SHOW_CI_TRY_AGAIN_MSGBOX,                     ///< show CI try again

    //channel list:
    EN_EXE_CHLIST_UP,                           ///< channel list up
    EN_EXE_CHLIST_DOWN,                           ///< channel list down
    EN_EXE_CHLIST_PAGE_UP,                           ///< channel list page up
    EN_EXE_CHLIST_PAGE_DOWN,                           ///< channel list page down
    EN_EXE_CHLIST_SELECT,                           ///< channel list select

    //Auto Scan
    EN_EXE_AUTO_SCAN_FINISH,          ///
    EN_EXE_SCAN_CONFIRM_BTN_YES,
    EN_EXE_SCAN_CONFIRM_BTN_NO,
    EN_EXE_SKIP_ATV_SCAN_CONFIRM_BTN_YES,
    EN_EXE_SKIP_ATV_SCAN_CONFIRM_BTN_NO,
    EN_EXE_SKIP_DTV_SCAN_CONFIRM_BTN_YES,
    EN_EXE_SKIP_DTV_SCAN_CONFIRM_BTN_NO,
    EN_EXE_SKIP_CATV_SCAN_CONFIRM_BTN_YES,
    EN_EXE_SKIP_CATV_SCAN_CONFIRM_BTN_NO,
    EN_EXE_OPEN_SCAN_SKIP_MENU,
    EN_EXE_OPEN_SCAN_EXIT_MENU,

    EN_EXE_SET_TARGET_REGION,
    EN_EXE_SET_TARGET_REGION_UP,
    EN_EXE_SET_TARGET_REGION_DOWN,
    EN_EXE_SET_TARGET_REGION_LEFT,
    EN_EXE_SET_TARGET_REGION_RIGHT,
    EN_EXE_SET_TARGET_REGION_SELECT,

    EN_EXE_SET_FAVORITE_NETWORK,
    EN_EXE_SET_FAVORITE_NETWORK_LEFT,
    EN_EXE_SET_FAVORITE_NETWORK_RIGHT,
    EN_EXE_SET_FAVORITE_NETWORK_SELECT,
    EN_EXE_FAVORITE_NETWORK_EXIT_SCAN_PAUSE,

    EN_EXE_SET_LCN_CONFLICT,
    EN_EXE_LCN_CONFILCT_CONFIRM_BTN_YES,
    EN_EXE_LCN_CONFILCT_CONFIRM_BTN_NO,
    EN_EXE_LCN_CONFLICT_SELECT_CM,
    EN_EXE_LCN_CONFLICT_EXIT_SCAN_PAUSE,
    EN_EXE_LCN_REFREASH_LCN_CONFILCT_MENU,

    //dtv manual scan
    EN_EXE_GET_PREV_CHANNEL,          ///< Get previous channel number
    EN_EXE_GET_NEXT_CHANNEL,           ///< Get next channel number
    EN_EXE_MANUAL_SCAN_CHANGE_RF,     ///< Manual scan change RF
    EN_EXE_DTV_MANUAL_SCAN_END,

    //dvbc scan input numbers
    EN_EXE_DVBC_SCAN_NUM0,
    EN_EXE_DVBC_SCAN_NUM1,
    EN_EXE_DVBC_SCAN_NUM2,
    EN_EXE_DVBC_SCAN_NUM3,
    EN_EXE_DVBC_SCAN_NUM4,
    EN_EXE_DVBC_SCAN_NUM5,
    EN_EXE_DVBC_SCAN_NUM6,
    EN_EXE_DVBC_SCAN_NUM7,
    EN_EXE_DVBC_SCAN_NUM8,
    EN_EXE_DVBC_SCAN_NUM9,

    //For DTV Manualscan 0~9
    EN_EXE_MANUAL_SCAN_NUMBERKEY_0,
    EN_EXE_MANUAL_SCAN_NUMBERKEY_1,
    EN_EXE_MANUAL_SCAN_NUMBERKEY_2,
    EN_EXE_MANUAL_SCAN_NUMBERKEY_3,
    EN_EXE_MANUAL_SCAN_NUMBERKEY_4,
    EN_EXE_MANUAL_SCAN_NUMBERKEY_5,
    EN_EXE_MANUAL_SCAN_NUMBERKEY_6,
    EN_EXE_MANUAL_SCAN_NUMBERKEY_7,
    EN_EXE_MANUAL_SCAN_NUMBERKEY_8,
    EN_EXE_MANUAL_SCAN_NUMBERKEY_9,

    //atv manual scan
    EN_EXE_GOTO_SCAN_DEC,                   ///< Search previous channel
    EN_EXE_GOTO_SCAN_INC,                   ///< Search next channel
    EN_EXE_ATV_MANUAL_SCAN_SHOW_INFO,     ///< Show frequency info.
    EN_EXE_ATV_MANUAL_SCAN_END,
    EN_EXE_ATV_MANUAL_SCAN_FINETUNE_UP,   ///< fine tune up
    EN_EXE_ATV_MANUAL_SCAN_FINETUNE_DOWN, ///< fine tune down
    EN_EXE_SAVE_ATV_MANUAL_SCAN,           ///< save atv manual scan

    EN_EXE_CHANGE_CURRENT_CH,
    EN_EXE_SET_MANUAL_SCAN,
    //0~9
    EN_EXE_CHANGE_CURRENT_CH_KEY_0,
    EN_EXE_CHANGE_CURRENT_CH_KEY_1,
    EN_EXE_CHANGE_CURRENT_CH_KEY_2,
    EN_EXE_CHANGE_CURRENT_CH_KEY_3,
    EN_EXE_CHANGE_CURRENT_CH_KEY_4,
    EN_EXE_CHANGE_CURRENT_CH_KEY_5,
    EN_EXE_CHANGE_CURRENT_CH_KEY_6,
    EN_EXE_CHANGE_CURRENT_CH_KEY_7,
    EN_EXE_CHANGE_CURRENT_CH_KEY_8,
    EN_EXE_CHANGE_CURRENT_CH_KEY_9,

    //program edit:
    EN_EXE_PREDIT_SELECT,                         ///< program edit select
    EN_EXE_PREDIT_UP,                               ///< program edit up
    EN_EXE_PREDIT_DOWN,                             ///< program edit down
    //EN_EXE_PREDIT_LEFT,                             ///< program edit left
    //EN_EXE_PREDIT_RIGHT,                            ///< program edit right
    EN_EXE_PREDIT_PAGE_UP,                         ///< program edit page up
    EN_EXE_PREDIT_PAGE_DOWN,                        ///< program edit page down
    EN_EXE_PREDIT_SKIP,                             ///< program edit skip
    EN_EXE_PREDIT_FAV,                              ///< program edit fav
    EN_EXE_PREDIT_LOCK,                             ///< program edit favorite
    EN_EXE_PREDIT_DELETE,                           ///< program edit delete
    EN_EXE_PREDIT_RENAME,                           ///< program edit rename
    EN_EXE_PREDIT_MOVE,                             ///< program edit move
    EN_EXE_PREDIT_MENU,                             ///< program edit menu key
    //for audio languages
    EN_EXE_AUDIO_LANG_LEFT,                   ///< audio language left
    EN_EXE_AUDIO_LANG_RIGHT,                    ///< audio language right
    EN_EXE_AUDIO_LANG_UP,                   ///< audio language up
    EN_EXE_AUDIO_LANG_DOWN,                    ///< audio language down

    //for subtitle languages
    EN_EXE_SUBTITLE_LANG_UP,                   ///< subtitle language up
    EN_EXE_SUBTITLE_LANG_DOWN,                    ///< subtitle language down

    //for EPG
    EN_EXE_EPG_SHOW_PROGRAMMEGUIDE_TIME_PAGE,///< show programme guide time page in EPG
    EN_EXE_EPG_SHOW_PROGRAMMEGUIDE_CHANNEL_PAGE,///< show programme guide channel page in EPG
    EN_EXE_EPG_SHOW_INFO,                       ///< show info page in EPG
    EN_EXE_EPG_SHOW_TIMER_LIST,                 ///< show timer list in EPG
    EN_EXE_EPG_SHOW_TIMER_MANUAL,               ///< show timer manual, in EPG
    EN_EXE_EPG_SHOW_TIMER_COUNTDOWN,            ///< show countdown timer, in EPG
    EN_EXE_EPG_TIMER_COUNTDOWN_UPDATE,          ///< show countdown timer, in EPG

    EN_EXE_EPG_SHOW_TIMER_RECORDER_LIST,         ///< show timer list in EPG
    EN_EXE_EPG_SHOW_RECORDER_SETTING,       ///< show timer manual, in EPG
    EN_EXE_EPG_SHOW_RECORD_TIME_SETTING,
    EN_EXE_EPG_CLOSE_RECORD_TIME_SETTING,
    EN_EXE_EPG_SHOW_TIMER_RECORDER_COUNTDOWN,    ///< show countdown timer, in EPG

    EN_EXE_EPG_RECORDER_SPLIT_ADJ,
    EN_EXE_EPG_RECORDER_SERIES_ADJ,

    EN_EXE_EPG_DATE_ADJ_DEC,                    ///< show main page in EPG decrease
    EN_EXE_EPG_DATE_ADJ_INC,                    ///< show main page in EPG increase

    EN_EXE_EPG_TIMER_PROG_ADJ_DEC,              ///< timer manual, in EPG decrease
    EN_EXE_EPG_TIMER_PROG_ADJ_INC,              ///< timer manual, in EPG increase
    EN_EXE_EPG_TIMER_MODE_ADJ_DEC,              ///< timer manual, in EPG decrease
    EN_EXE_EPG_TIMER_MODE_ADJ_INC,              ///< timer manual, in EPG increase

    EN_EXE_EPG_TIMER_MON_ADJ_DEC,               ///< timer manual, in EPG decrease
    EN_EXE_EPG_TIMER_MON_ADJ_INC,               ///< timer manual, in EPG increase
    EN_EXE_EPG_TIMER_DATE_ADJ_DEC,              ///< timer manual, in EPG decrease
    EN_EXE_EPG_TIMER_DATE_ADJ_INC,              ///< timer manual, in EPG increase
    EN_EXE_EPG_TIMER_HOUR_ADJ_DEC,              ///< timer manual, in EPG decrease
    EN_EXE_EPG_TIMER_HOUR_ADJ_INC,              ///< timer manual, in EPG increase
    EN_EXE_EPG_TIMER_MIN_ADJ_DEC,               ///< timer manual, in EPG decrease
    EN_EXE_EPG_TIMER_MIN_ADJ_INC,               ///< timer manual, in EPG increase
    EN_EXE_EPG_REMINDER_SAVE,                   ///< timer manual, in EPG save

    EN_EXE_EPG_SHOW_RECORDER_TIMER_LIST,
    EN_EXE_EPG_SHOW_RECORDER_TIMER_COUNTDOWN,
    EN_EXE_EPG_RECORDER_MODE_ADJ_DEC,     ///< timer manual, in EPG decrease
    EN_EXE_EPG_RECORDER_MODE_ADJ_INC,     ///< timer manual, in EPG increase
    EN_EXE_EPG_RECORDER_PROG_ADJ_DEC,
    EN_EXE_EPG_RECORDER_PROG_ADJ_INC,
    EN_EXE_EPG_RECORDER_YEAR_ADJ_DEC,
    EN_EXE_EPG_RECORDER_YEAR_ADJ_INC,
    EN_EXE_EPG_RECORDER_MONTH_ADJ_DEC,      ///< timer manual, in EPG decrease
    EN_EXE_EPG_RECORDER_MONTH_ADJ_INC,      ///< timer manual, in EPG increase
    EN_EXE_EPG_RECORDER_DAY_ADJ_DEC,     ///< timer manual, in EPG decrease
    EN_EXE_EPG_RECORDER_DAY_ADJ_INC,     ///< timer manual, in EPG increase
    EN_EXE_EPG_RECORDER_HOUR_ADJ_DEC,     ///< timer manual, in EPG decrease
    EN_EXE_EPG_RECORDER_HOUR_ADJ_INC,     ///< timer manual, in EPG increase
    EN_EXE_EPG_RECORDER_MIN_ADJ_DEC,      ///< timer manual, in EPG decrease
    EN_EXE_EPG_RECORDER_MIN_ADJ_INC,      ///< timer manual, in EPG increase
    EN_EXE_EPG_RECORDER_SETTING_SAVE,             ///< timer manual, in EPG save

    EN_EXE_EPG_TIMER_DELETE_CURRENT,
    EN_EXE_EPG_TIMER_SAVE_DLG_CLOSE,
    EN_EXE_EPG_RECORDER_DELETE,
    EN_EXE_EPG_RECORDER_SAVE_DLG_CLOSE,
    EN_EXE_EPG_RECORDER_POPUP_YES,
    EN_EXE_EPG_RECORDER_POPUP_NO,

    EN_EXE_EPG_TIMER_COUNTDOWN_BTN,
    EN_EXE_EPG_TIMER_COUNTDOWN_BTN_YES,
    EN_EXE_EPG_TIMER_COUNTDOWN_BTN_NO,

    EN_EXE_EPG_TIMERLIST_PREVITEM,
    EN_EXE_EPG_RECORDER_LIST_PREVITEM,
    EN_EXE_EPG_TIMERLIST_NEXTITEM,
    EN_EXE_EPG_RECORDER_LIST_NEXTITEM,
    EN_EXE_EPG_RECORDER_ALTERNATIVE,
    EN_EXE_EPG_RECORDER_ALTERNATIVE_OK,
    EN_EXE_EPG_RECORDER_ALTERNATIVE_CANCEL,
    //for factory menu:
    EN_EXE_FACTORY_MENU_ADJ_DEC,                 ///< factory menu decrease
    EN_EXE_FACTORY_MENU_ADJ_INC,                 ///< factory menu increase
    EN_EXE_FACTORY_MENU_PREV_PAGE,                 ///< factory menu goto previous page
    EN_EXE_FACTORY_MENU_NEXT_PAGE,                 ///< factory menu goto next page
    EN_EXE_QMAP_PREV_PAGE,    ///< QMAP menu goto previous page
    EN_EXE_QMAP_NEXT_PAGE,    ///< QMAP menu goto next page
    EN_EXE_QMAP_PAGE_DOWN,    ///< QMAP page down
    EN_EXE_QMAP_PAGE_UP,      ///< QMAP page up
    EN_EXE_FACTORY_MENU_GOTO_BAR_ADJUST,           ///< factory menu goto bar page
    EN_EXE_FACTORY_MENU_EXIT_BAR_ADJUST,           ///< factory menu Exit bar page
    EN_EXE_BIST_TEST,                               ///< BIST TEST

    //for expert menu:
    EN_EXE_EXPERT_MENU_ADJ_DEC,                 ///< EXPERT menu decrease
    EN_EXE_EXPERT_MENU_ADJ_INC,                 ///< EXPERT menu increase
    EN_EXE_EXPERT_MENU_PREV_PAGE,                 ///< EXPERT menu goto previous page
    EN_EXE_EXPERT_MENU_NEXT_PAGE,                 ///< EXPERT menu goto next page
    EN_EXE_EXPERT_MENU_GOTO_BAR_ADJUST,           ///< expert menu goto bar page
    EN_EXE_EXPERT_MENU_EXIT_BAR_ADJUST,           ///< expert menu Exit bar page
    EN_EXE_EXPERT_PREV_PAGE,                    ///< expert menu up
    EN_EXE_EXPERT_NEXT_PAGE,                    ///< expert menu down

    //misc:
    EN_EXE_GOTO_SWITCH_INPUTSOURCE,
    EN_EXE_SWITCH_INPUTSOURCE,                  ///< apply and switch input source


//    EN_EXE_MUTE_AUDIO_VOLUME,                   ///< mute audio volume
    EN_EXE_GOTO_MAIN_MENU,
    EN_EXE_RESET_AUTO_CLOSE_TIMER,              ///< reset auto close timer
    EN_EXE_GOTO_PIC_ADJUST_CONTRAST,            ///< goto PIC adjust CONTRAST
    EN_EXE_GOTO_PIC_ADJUST_BRIGHTNESS,          ///< goto PIC adjust BRIGHTNESS
    EN_EXE_GOTO_PIC_ADJUST_COLOR,               ///< goto PIC adjust COLOR
    EN_EXE_GOTO_PIC_ADJUST_SHARPNESS,           ///< goto PIC adjust SHARPNESS
    EN_EXE_GOTO_PIC_ADJUST_TINT,                ///< goto PIC adjust Tint
    EN_EXE_GOTO_PIC_ADJUST_TEMP_RED,            ///< goto PIC adjust TEMP_RED
    EN_EXE_GOTO_PIC_ADJUST_TEMP_GREEN,          ///< goto PIC adjust TEMP_GREEN
    EN_EXE_GOTO_PIC_ADJUST_TEMP_BLUE,           ///< goto PIC adjust TEMP_BLUE
    EN_EXE_GOTO_PC_ADJUST,                      ///< goto pc adjust menu
    EN_EXE_GOTO_ASPECT_RATIO_PAGE,              ///< goto aspect ratio page
    EN_EXE_GOTO_PARENTAL_GUIDANCE_PAGE,         ///< goto parental guidance page
    EN_EXE_GOTO_NOISE_REDUCTION_PAGE,           ///< goto noise redction page
    EN_EXE_GOTO_SURROUND_SOUND_PAGE,            ///< goto surrund sound page
    EN_EXE_GOTO_SLEEP_TIMER_PAGE,               ///< goto sleep timer page
    EN_EXE_GOTO_SET_CLOCK,                      ///< goto set clock dialog
    EN_EXE_EXECUTE_APP,
    EN_EXE_CLOSE_SET_CLOCK,                     ///< close set clock dialog
    EN_EXE_CLOSE_SET_OFFTIME,                   ///<close set off time dialog
    EN_EXE_CLOSE_SET_ONTIME,                    ///<close set on time dialog
    EN_EXE_CLOSE_SET_LANGUAGE,              ///<close set LANGUAGE dialog
    EN_EXE_GOTO_SET_OFF_TIMER,                  ///< goto set off timer dialog
    EN_EXE_GOTO_SET_ON_TIMER,                   ///< goto set on timer
    EN_EXE_GOTO_SET_AUDIO_LAN,                 ///< goto set audio language
    EN_EXE_GOTO_AUDIOLANG_PAGE,                ///< goto audiolang page
    EN_EXE_GOTO_SUBLANG_PAGE,                  ///< goto sublang page
    EN_EXE_GOTO_CONFIRM_FACTORY_RESET,        ///< goto MENU_CONFIRM_FACTORY_PAGE
    EN_EXE_GOTO_FACTORY_RESET_CONFIRM_DLG,       ///< goto factory reset confirm dialog
    EN_EXE_GOTO_CONFIRM_DIVX,                   ///<goto DRM DivX PAGE
    EN_EXE_GOTO_DIVX_DLG,               ///<goto DRM DivX dialog
    EN_EXE_GOTO_CONFIRM_DEACTIVATION,           ///<goto DRM Deactivation PAGE
    EN_EXE_GOTO_DEACTIVATION_CONFIRM_DLG,       ///<goto DRM Deactivation confirm dialog
    EN_EXE_PASS_PASSWORD,                        ///< goto pages after pasing password
    EN_EXE_CLOSE_FACTORY_RESET_CONFIRM_DLG,      ///< close factory reset confirm dialog
    EN_EXE_CLOSE_DIVX_CONFIRM_DLG,              ///<close DRM DivX confirm dialog
    EN_EXE_CLOSE_DEACTIVATION_CONFIRM_DLG,       ///<close DRM Deactivation confirm dialog
    EN_EXE_CLOSE_USB_UPGRADE_CONFIRM_DLG,      ///< close USB upgrade confirm dialog
    EN_EXE_GOTO_COMMON_DLG_YES,              ///< goto common dialog ,press YES key
    EN_EXE_GOTO_COMMON_DLG_NO,              ///< goto common dialog ,press NO key
    EN_EXE_FACTORY_RESET,                       ///< execute factory reset
    EN_EXE_USER_SETTING_RESET,                  ///< execute main menu user setting reset
    EN_EXE_USB_UPGRADE_FAILED,                       ///< upgrade failed
    EN_EXE_GOTO_INPUT_PASSWORD_DLG,                 ///< goto input password dialog
    EN_EXE_CLOSE_INPUT_PASSWORD_DLG,                ///< close input password dialog
    EN_EXE_GOTO_SET_PASSWORD_DLG,                  ///< goto set password dialog
    EN_EXE_ENTER_MENU_LOCK_PAGE,                     ///< enter MainMenu_Lock_page
    EN_EXE_CLOSE_SET_PASSWORD_DLG,                  ///< close set password dialog
    EN_EXE_GOTO_WRONG_PASSWORD_DLG,             ///< goto wrong password message box
    EN_EXE_CLOSE_WRONG_PASSWORD_DLG,            ///< close wrong password message box
    EN_EXE_GOTO_MISMATCH_PASSWORD_DLG,          ///< goto mismatch password message box
    EN_EXE_CLOSE_MISMATCH_PASSWORD_DLG,         ///< close mismatch password message box
    EN_EXE_CHECK_INPUT_PASSWORD,                ///< check input password is right or not...
    EN_EXE_CHECK_SET_PASSWORD_CHECKOLDPW,       ///< check old password in set password page
    EN_EXE_CHECK_SET_PASSWORD,                  ///< check set password is right or not...
    EN_EXE_CLEAR_PASSWORD,                      ///< clear password input data (input/change)
    EN_EXE_GOTO_SCAN_INPUT_PASSWORD_DLG,       ///< goto scan lock password confirm dialog
    EN_EXE_GOTO_SCAN_INPUT_PASSWORD_DLG_FORM_DVBC,      ///<goto scan lock password confirm dialog from DVBC type by user setting
    EN_EXE_GOTO_DVB_SELECT_INPUT_PASSWORD_DLG,       ///< goto DVB select type manual tuninglock password confirm dialog
    EN_EXE_GOTO_DTV_TUNING_INPUT_PASSWORD_DLG,  ///< goto DTV manual tuning lock password confirm dialog
    EN_EXE_GOTO_ATV_TUNING_INPUT_PASSWORD_DLG,  ///< goto ATV manual tuning lock password confirm dialog
    EN_EXE_GOTO_CADTV_TUNING_INPUT_PASSWORD_DLG,  ///< goto ATV manual tuning lock password confirm dialog
    EN_EXE_GOTO_DVBC_SCAN_PAGE,                      ///< goto DVB-C Scan page
    EN_EXE_GOTO_TUNING_CONFIRM,                 ///< goto tuning confirm dialog
    EN_EXE_GOTO_MENU_LOCK_PAGE,                 ///< goto confirm passwrod and enter menu_lock_page
    EN_EXE_CLOSE_TUNING_CONFIRM,                ///< close tuning confirm dialog
    EN_EXE_CLOSE_DVB_SELECT,                        ///< close DVB select dialog
    EN_EXE_CLOSE_PCMODE_PAGE,                   ///< close pc-mode page, return to picture page
    EN_EXE_PCMODE_AUTO_ADJ,                     ///< execute pc-mode auto adjust
    //EN_EXE_SWITCH_AUDIOLANG,                    ///< switch audio language
    EN_EXE_CIMMI_MENU_ITEM_SELECT,              ///< CI/MMI item selection
    EN_EXE_CIMMI_INPUT_PIN_SELECT,              ///< CI/MMI input pin selected
    EN_EXE_GOTO_SET_SUBTITLE_LAN,               ///< goto set subtitle language
    EN_EXE_SET_AUTOTUNING_COUNTRY,              ///< goto set set country
    EN_EXE_GOTO_SET_OSD_LAN,                    ///< goto set OSD language
    EN_EXE_GOTO_SET_TIMEZONE,

    EN_EXE_SET_DSC_KEY_ARC4X3,                  ///< hotkey scale: set 4:3
    EN_EXE_SET_DSC_KEY_ARC16X9,                 ///< hotkey scale: set 16:9
    EN_EXE_SET_DSC_KEY_ARCZOOM,                 ///< hotkey scale: set zoom
    EN_EXE_ZOOM_ARROW_UP,                       ///< hotkey scale: key up
    EN_EXE_ZOOM_ARROW_DOWN,                     ///< hotkey scale: key down
    EN_EXE_ZOOM_ARROW_LEFT,                     ///< hotkey scale: key left
    EN_EXE_ZOOM_ARROW_RIGHT,                    ///< hotkey scale: key right

    //#ifdef ENABLE_KTV
    EN_EXE_KTV_ENTER,
    EN_EXE_KTV_EXIT,
    EN_EXE_KTV_SOFTWARW_KEYBOARD_DIGITINPUT,
    EN_EXE_KTV_SOFTWARW_KEYBOARD_LETTERINPUT,
    EN_EXE_KTV_VOLUME_DECREASE,
    EN_EXE_KTV_VOLUME_INCREASE,
    EN_EXE_KTV_SINGERITEM_DECREASE,
    EN_EXE_KTV_SINGERITEM_INCREASE,
    EN_EXE_KTV_SONGITEM_DECREASE,
    EN_EXE_KTV_SONGITEM_INCREASE,
    EN_EXE_KTV_SONG_FIRSTWORD_ITEM_DECREASE,
    EN_EXE_KTV_SONG_FIRSTWORD_ITEM_INCREASE,
    EN_EXE_KTV_SONG_FIRSTWORD_ITEM_LEFT,
    EN_EXE_KTV_SONG_FIRSTWORD_ITEM_RIGHT,
    EN_EXE_KTV_SONG_FIRSTWORD_ITEM_PAGEDOWN,
    EN_EXE_KTV_SONG_FIRSTWORD_ITEM_PAGEUP,
    EN_EXE_KTV_SETUP_ITEM_DECREASE,
    EN_EXE_KTV_SETUP_ITEM_INCREASE,
    EN_EXE_KTV_PLAYLIST_DECREASE,
    EN_EXE_KTV_PLAYLIST_INCREASE,
    EN_EXE_KTV_KEYUP_TO_LIST,
    EN_EXE_KTV_FAVOR_KEYRIGHT,
    //#endif

    //#if(ENABLE_PVR ==1)
    EN_EXE_GOTO_PVR_SELECT_DISK_PANE,
    EN_EXE_SELECT_DISK_GOTO_PVR_BG_PANE,
    EN_EXE_PVR_SELECT_DISK_USB_LEFT,
    EN_EXE_PVR_SELECT_DISK_USB_RIGHT,
    EN_EXE_PVR_SELECT_DISK_DISK_LEFT,
    EN_EXE_PVR_SELECT_DISK_DISK_RIGHT,
    EN_EXE_PVR_SELECT_DISK_SET_DISK,
    EN_EXE_PVR_SELECT_DISK_RESTORE_DISK,
    EN_EXE_PVR_CREATE_PARTITION_START,
    EN_EXE_PVR_SELECT_FREE_REC_LIMIT_LEFT,
    EN_EXE_PVR_SELECT_FREE_REC_LIMIT_RIGHT,
    EN_EXE_CREATE_PARTITION_GOTO_PVR_BG_PANE,
    EN_EXE_GOTO_PVR_CHECK_FS_START,
    EN_EXE_GOTO_PVR_CHECK_FS,
    EN_EXE_GOTO_PVR_FILE_SYSTEM_SIZE_PANE,
    EN_EXE_DEC_FILE_SYSTEM_TYPE,
    EN_EXE_INC_FILE_SYSTEM_TYPE,
    EN_EXE_DEC_FILE_SYSTEM_SIZE,
    EN_EXE_INC_FILE_SYSTEM_SIZE,
    EN_EXE_DEC_TIME_SHIFT_DISK_SIZE,
    EN_EXE_INC_TIME_SHIFT_DISK_SIZE,
    EN_EXE_FILE_SYSTEM_SIZE_GOTO_PVR_BG_PANE,
    EN_EXE_TIME_SHIFT_SIZE_GOTO_PVR_BG_PANE,
    EN_EXE_FILE_SYSTEM_SIZE_START_FORMAT_CHECK,
    EN_EXE_FILE_SYSTEM_SIZE_START_FORMAT,
    EN_EXE_CHANGE_TO_CREATE_PVR_FILE_SYSTEM_PLEASE_WAIT_TXT,
    EN_EXE_TIME_SHIFT_SIZE_FORMAT,
    EN_EXE_REPAINT_PVR_FORMAT_PROGRESS,
    EN_EXE_SWITCH_INPUTSOURCE_CONFIRM_OK,
    EN_EXE_SWITCH_INPUTSOURCE_CONFIRM_CANCEL,
    EN_EXE_DLG_CONFIRM_OK,
    EN_EXE_DLG_CONFIRM_CANCEL,
    //#endif

    EN_EXE_MSGBOX_BTN_OK,
    EN_EXE_MSGBOX_BTN_OK_GO_LEFT,
    EN_EXE_MSGBOX_BTN_CLEAR,
    EN_EXE_MSGBOX_BTN_CLEAR_GO_RIGHT,

    //#if DVB_C_ENABLE
    EN_EXE_CADTV_CHANGE_FREQUENCY_LEFT,
    EN_EXE_CADTV_CHANGE_FREQUENCY_RIGHT,
    EN_EXE_CADTV_CHANGE_FREQUENCY,
    EN_EXE_CADTV_CHANGE_SYMBOL_LEFT,
    EN_EXE_CADTV_CHANGE_SYMBOL_RIGHT,
    EN_EXE_CADTV_CHANGE_SYMBOL,
    EN_EXE_CADTV_CHANGE_QAMTYPE_LEFT,
    EN_EXE_CADTV_CHANGE_QAMTYPE_RIGHT,
    EN_EXE_CADTV_START_SEARCH,
    EN_EXE_CADTV_MANUAL_SCAN_END,

    EN_EXE_CADTV_NUM0,
    EN_EXE_CADTV_NUM1,
    EN_EXE_CADTV_NUM2,
    EN_EXE_CADTV_NUM3,
    EN_EXE_CADTV_NUM4,
    EN_EXE_CADTV_NUM5,
    EN_EXE_CADTV_NUM6,
    EN_EXE_CADTV_NUM7,
    EN_EXE_CADTV_NUM8,
    EN_EXE_CADTV_NUM9,
    //#endif

    //for network config page
    EN_EXE_CLOSE_NET_CONFIG,

    //for network ip
    EN_EXE_DEC_NET_IP_A,
    EN_EXE_INC_NET_IP_A,
    EN_EXE_DEC_NET_IP_B,
    EN_EXE_INC_NET_IP_B,
    EN_EXE_DEC_NET_IP_C,
    EN_EXE_INC_NET_IP_C,
    EN_EXE_DEC_NET_IP_D,
    EN_EXE_INC_NET_IP_D,
    EN_EXE_CLOSE_NET_IP,

    //for network netmask
    EN_EXE_DEC_NET_NETMASK_A,
    EN_EXE_INC_NET_NETMASK_A,
    EN_EXE_DEC_NET_NETMASK_B,
    EN_EXE_INC_NET_NETMASK_B,
    EN_EXE_DEC_NET_NETMASK_C,
    EN_EXE_INC_NET_NETMASK_C,
    EN_EXE_DEC_NET_NETMASK_D,
    EN_EXE_INC_NET_NETMASK_D,
    EN_EXE_CLOSE_NET_NETMASK,

    //for network gateway
    EN_EXE_DEC_NET_GATEWAY_A,
    EN_EXE_INC_NET_GATEWAY_A,
    EN_EXE_DEC_NET_GATEWAY_B,
    EN_EXE_INC_NET_GATEWAY_B,
    EN_EXE_DEC_NET_GATEWAY_C,
    EN_EXE_INC_NET_GATEWAY_C,
    EN_EXE_DEC_NET_GATEWAY_D,
    EN_EXE_INC_NET_GATEWAY_D,
    EN_EXE_CLOSE_NET_GATEWAY,

    //for network DNS
    EN_EXE_DEC_NET_DNS_A,
    EN_EXE_INC_NET_DNS_A,
    EN_EXE_DEC_NET_DNS_B,
    EN_EXE_INC_NET_DNS_B,
    EN_EXE_DEC_NET_DNS_C,
    EN_EXE_INC_NET_DNS_C,
    EN_EXE_DEC_NET_DNS_D,
    EN_EXE_INC_NET_DNS_D,
    EN_EXE_CLOSE_NET_DNS,

    //for new effect
    EN_EXE_EFFECT_POPUP,
    EN_EXE_EFFECT_SLIDEITEM,
    EN_EXE_FOCUSITEM_UP,
    EN_EXE_FOCUSITEM_DOWN,
    EN_EXE_EFFECT_FLIPPAGE_LEFT,
    EN_EXE_EFFECT_FLIPPAGE_RIGHT,

    EN_EXE_EFFECT_SPREADOUT,
    EN_EXE_EFFECT_ROLLUP,
    EN_EXE_EFFECT_ZOOM_IN,
    EN_EXE_EFFECT_ZOOM_OUT,

    //for query....
    EN_EXE_QUERY_IS_POWER_OFF_COUNTDOWN_MSG_BOX,      ///< query if showing power off countdown msg box
    EN_EXE_QUERY_IS_PASSWORD_INPUT_MSG_BOX,

    // for 3D effect setting
    EN_EXE_INC_MAIN_PAGE,
    EN_EXE_DEC_MAIN_PAGE,
    EN_EXE_INC_MAIN_MENU_OPEN,
    EN_EXE_DEC_MAIN_MENU_OPEN,
    EN_EXE_INC_MAIN_MENU_CLOSE,
    EN_EXE_DEC_MAIN_MENU_CLOSE,
    EN_EXE_INC_MAIN_MENU_ITEM,
    EN_EXE_DEC_MAIN_MENU_ITEM,
    EN_EXE_INC_POPUP_DIALOG,
    EN_EXE_DEC_POPUP_DIALOG,
    EN_EXE_INC_ASSISTANT_MENU,
    EN_EXE_DEC_ASSISTANT_MENU,
    EN_EXE_INC_DYNAMIC_TEXT_EFFECT,
    EN_EXE_DEC_DYNAMIC_TEXT_EFFECT,

    //monitor idle page
    EN_EXE_IDLE_START_MAINMENU,
    EN_EXE_IDLE_BUTTON_CLICK,
    EN_EXE_IDLE_BALL_PROGRESS_INC,
    EN_EXE_IDLE_BALL_PROGRESS_DEC,
    EN_EXE_IDLE_RECT_PROGRESS_INC,
    EN_EXE_IDLE_RECT_PROGRESS_DEC,
    EN_EXE_IDLE_DYNAMIC_TEXT_CHANGE,
    EN_EXE_IDLE_DYNAMIC_COLOR_CHANGE,
    EN_EXE_IDLE_DYNAMIC_BITMAP_CHANGE,
    EN_EXE_IDLE_DYNAMIC_LIST_CHANGE,
    EN_EXE_IDLE_DYNAMIC_TEXT_EFFECT_CHANGE,
    EN_EXE_IDLE_MARQUEE_START,

    //monitor mainmenu

    EN_EXE_GOTO_NEXT_PAGE,
    EN_EXE_GOTO_NEXT_KEY_UP,
    EN_EXE_GOTO_NEXT_KEY_DOWN,
    EN_EXE_GOTO_PREVIOUS,
    EN_EXE_GOTO_ADJUST_VALUE,
    EN_EXE_ADJUST_VALUE_INC_KEY,
    EN_EXE_ADJUST_VALUE_DEC_KEY,
    EN_EXE_GOTO_SELECT_VALUE,
    EN_EXE_CLEAN_VALUE_UP_KEY,
    EN_EXE_CLEAN_VALUE_DOWN_KEY,
    EN_EXE_ADJUST_ITEM_DOWN_KEY,
    EN_EXE_GOTO_ENTER,

    //monitor mainmenu [for OSD Manager]
    EN_EXE_NEXT_PAGE,
    EN_EXE_PREVIOUS_PAGE,
    EN_EXE_KEY_UP_CHANGE_VALUE,
    EN_EXE_KEY_DOWN_CHANGE_VALUE,
    EN_EXE_ADJUST_VALUE,
    EN_EXE_ADJUST_MENU_INCREASE,
    EN_EXE_ADJUST_MENU_DECREASE,
    EN_EXE_SELECT_CUSTOM_ITEM,
    EN_EXE_SELECT_ITEM,
    EN_EXE_CANCEL_ADJUST_VALUE,
    EN_EXE_SAVE_VALUE,
    EN_EXE_KEY_UP_PAGE,
    EN_EXE_KEY_DOWN_PAGE,

    EN_EXE_MAX,
} EN_OSD_WNDEXEACTION_ENUM;

