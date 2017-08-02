///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mdrv_tmds.c
/// @author MStar Semiconductor Inc.
/// @brief  TMDS driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_TMDS_C_
#define _MDRV_TMDS_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "Global.h"
#include <string.h>
#include "mdrv_tmds.h"
#include "mhal_tmds.h"
#include "mhal_tmds.c"

#define TMDS_PrtMsg(str)                    //printMsg(str)
#define TMDS_PrtData(str, x)                //printData(str, x)

#ifdef _WORK_ON_PM_
MsCombo_TMDS_Port tmds_port[6];
BYTE tmds_hdmi_port;
BYTE tmds_dvi_port;
BYTE tmds_dual_port;

void mdrv_tmds_CheckAudioErrorStatus(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    tmds_port[enInputPort].AudioErrorStatus = mhal_tmds_CheckAudioErrorStatus(enInputPort);
}

void mdrv_tmds_HDMIGetPktStatus(MSCombo_TMDS_PORT_INDEX enInputPort)
{
    tmds_port[enInputPort].InputPacketStatus = mhal_tmds_HDMIGetPktStatus(enInputPort);
    tmds_port[enInputPort].InputPacketError= mhal_tmds_HDMIGetErrorStatus(enInputPort);

	if( tmds_port[enInputPort].InputPacketStatus & BIT3)
        tmds_port[enInputPort].InputAVIInfoReceived = 1;

    if( tmds_port[enInputPort].InputPacketStatus & BIT7)
        tmds_port[enInputPort].InputVSPktReceived = 1;
}

void mdrv_tmds_PowerControl(MSCombo_TMDS_PORT_INDEX enInputPort, MS_BOOL bFlag)
{
    mhal_tmds_PowerControl(enInputPort, bFlag);
}


void mdrv_tmds_SetConfiguration(BYTE ucDVIEnableIndex, BYTE ucDualEnableIndex, BYTE ucHDMIEnableIndex, BYTE ucDisplayEnableIndex)
{
    MSCombo_TMDS_PORT_INDEX i;

    tmds_dvi_port = ucDVIEnableIndex;
    tmds_dual_port = ucDualEnableIndex;
    tmds_hdmi_port = ucHDMIEnableIndex;

    for(i = MSCombo_TMDS_PORT_0; i <= MSCombo_TMDS_PORT_MAX; i++)
    {
        if(ucDisplayEnableIndex & (1 << i))
		    tmds_port[i].port_power= MsCombo_TMDS_POWER_ON;
        else
            tmds_port[i].port_power= MsCombo_TMDS_POWER_OFF;
    }
}

void mdrv_tmds_Initial(void)
{
    MSCombo_TMDS_PORT_INDEX i;

    for(i = MSCombo_TMDS_PORT_0; i <= MSCombo_TMDS_PORT_MAX; i++)
    {
        if(tmds_dvi_port & (1 << i))
        {
            tmds_port[i].port_type = MsCombo_TMDS_TYPE_DVI;
            tmds_port[i].input_color.ucColorType = COMBO_COLOR_FORMAT_RGB;

            mhal_tmds_Initial(i, FALSE);
        }
        else if(tmds_dual_port & (1 << i))
        {
            tmds_port[i].port_type = MsCombo_TMDS_TYPE_DUAL;
            tmds_port[i].input_color.ucColorType = COMBO_COLOR_FORMAT_RGB;

            mhal_tmds_Initial(i, TRUE);
        }
        else if(tmds_hdmi_port & (1 << i))
        {
            tmds_port[i].port_type = MsCombo_TMDS_TYPE_HDMI;
            tmds_port[i].input_color.ucColorType = COMBO_COLOR_FORMAT_DEFAULT;

            mhal_tmds_Initial(i, FALSE);
        }
        else
        {
            tmds_port[i].port_type = MsCombo_TMDS_TYPE_RESERVED;
            tmds_port[i].input_color.ucColorType = COMBO_COLOR_FORMAT_UNKNOWN;
        }
        tmds_port[i].input_color.ucColorimetry = COMBO_COLORIMETRY_NONE;
        tmds_port[i].input_color.ucColorRange = COMBO_COLOR_RANGE_DEFAULT;

        tmds_port[i].AudioOutputState = TMDS_AUDIO_STATE_OFF;
        tmds_port[i].DataOutputState = TMDS_DATA_OUTPUT_OFF;
        tmds_port[i].AudioOutputEnable = FALSE;
        tmds_port[i].InputPacketStatus = 0;
        tmds_port[i].InputPacketError = 0;
        tmds_port[i].ModeChange = 0;
        tmds_port[i].ColorChange = 0;
        tmds_port[i].InputVideoMode = 0;
		tmds_port[i].InputAVIInfoReceived = 0;
        tmds_port[i].InputVSPktReceived = 0;
		tmds_port[i].AVI_Colorimetry = 0;
		tmds_port[i].AVI_PktLossCnt = 0;
        tmds_port[i].VS_PktLossCnt = 0;
        tmds_port[i].AudioErrorStatus = 0;
        tmds_port[i].AudioOffDebunce = 0;
    }


}

void mdrv_tmds_ConfigAudioPort(BYTE ucComboPort)
{
    BYTE i;

    mhal_tmds_ConfigAudioPort(ucComboPort);
    tmds_port[ucComboPort].AudioOutputEnable = TRUE;

    //clear other ports' status
    for (i=0; i < MSCombo_TMDS_PORT_MAX; i++)
    {
        if (i != ucComboPort)
            tmds_port[i].AudioOutputEnable = FALSE;
    }

}

void mdrv_tmds_EnableAudioMute(MsCombo_TMDS_AUD_STATUS AudioStatus)
{
    if (AudioStatus == TMDS_AUDIO_STATE_OFF)
        mhal_tmds_EnableAudioMute(TRUE);
    else mhal_tmds_EnableAudioMute(FALSE);
}

void mdrv_tmds_CheckAudio(BYTE ucComboPort)
{
    MsCombo_TMDS_AUD_STATUS TmpAudStatus;

    //b'4: audio sample error; b'5:  audio sample polarity error;
    if (tmds_port[ucComboPort].InputPacketError & (BIT4 | BIT5))
        TmpAudStatus = TMDS_AUDIO_STATE_OFF;

    //b'5: reg_asample_pkt; b'6: reg_acr_pkt
    else if ((tmds_port[ucComboPort].InputPacketStatus & (BIT5 | BIT6)) == (BIT5 | BIT6) )
    {
        if (tmds_port[ucComboPort].DataOutputState == TMDS_DATA_OUTPUT_ON)
            TmpAudStatus = TMDS_AUDIO_STATE_ON;
        else TmpAudStatus = TMDS_AUDIO_STATE_OFF;
    }
    else if (tmds_port[ucComboPort].AudioErrorStatus == 0)
        TmpAudStatus = TMDS_AUDIO_STATE_ON;
    else TmpAudStatus = TMDS_AUDIO_STATE_OFF;

    if( tmds_port[ucComboPort].AudioOutputState == TMDS_AUDIO_STATE_ON && TmpAudStatus == TMDS_AUDIO_STATE_OFF )
    {
        if( ++tmds_port[ucComboPort].AudioOffDebunce < 20 )
            return ;
    }
    else
        tmds_port[ucComboPort].AudioOffDebunce = 0;

    if( TmpAudStatus != tmds_port[ucComboPort].AudioOutputState )
    {
        mdrv_tmds_EnableAudioMute(TmpAudStatus);
        tmds_port[ucComboPort].AudioOutputState = TmpAudStatus;
    }

}

void mdrv_tmds_Handler(void)
{
    static MSCombo_TMDS_PORT_INDEX i = MSCombo_TMDS_PORT_0;

    if(tmds_port[i].port_type == MsCombo_TMDS_TYPE_HDMI)
    {
        if(tmds_port[i].port_power == MsCombo_TMDS_POWER_ON) // Power On
        {
            if(mhal_tmds_DE_Stable(i)) //DE stable
            {
                tmds_port[i].InputVideoMode = mhal_tmds_GetInputType(i);
                if(tmds_port[i].InputVideoMode) //  Input is HDMI
                {
                    tmds_port[i].InputAVIInfoReceived = 0;
                    tmds_port[i].InputVSPktReceived = 0;
                    tmds_port[i].DataOutputState = TMDS_DATA_OUTPUT_ON;

                    mdrv_tmds_HDMIGetPktStatus(i);

					if(tmds_port[i].InputAVIInfoReceived)
                    {
                        tmds_port[i].AVI_PktLossCnt = 0;
                        tmds_port[i].input_color = (ST_COMBO_COLOR_FORMAT)mhal_tmds_GetColor(i);
                    }
					else
					{
						if(tmds_port[i].AVI_PktLossCnt>AviPktLossBound) //temp
                        {
                            tmds_port[i].AVI_PktLossCnt = 0;
							tmds_port[i].input_color.ucColorType = COMBO_COLOR_FORMAT_RGB;
                        }
                    }

                    if (tmds_port[i].InputVSPktReceived)
                        tmds_port[i].VS_PktLossCnt = 0;
                    else
                    {
                        if (tmds_port[i].VS_PktLossCnt > VS_PKT_LOSS_BOUND)
                            tmds_port[i].VS_PktLossCnt = 0;
                    }

                    if (tmds_port[i].AudioOutputEnable== TRUE)
                    {
                        mdrv_tmds_CheckAudio(i);

                        #if (ENABLE_AUDIO_AUTO_MUTE || ENABLE_AUDIO_AUTO_FADING)
                            mdrv_tmds_CheckAudioErrorStatus(i);
                        #endif
                    }
                }
                else // Input is DVI
                {
                    if(mhal_tmds_TimingChg(i))
                    {
                        tmds_port[i].input_color.ucColorType = COMBO_COLOR_FORMAT_RGB;
                        mhal_tmds_reset_status(i, TRUE, 0xDD);
                    }
                }
            }
            else  //DE unstable
            {
                //do nothing.
            }
        }
        else // Power off
        {
            //mdrv_tmds_PowerControl(i, FALSE);
            tmds_port[i].InputPacketStatus = 0;
		    tmds_port[i].InputPacketError = 0;
            tmds_port[i].ModeChange = 0;
            tmds_port[i].ColorChange = 0;
            tmds_port[i].InputVideoMode= 0;
            tmds_port[i].AVI_PktLossCnt = 0;
            tmds_port[i].VS_PktLossCnt = 0;
        }
    }
    else if(tmds_port[i].port_type == MsCombo_TMDS_TYPE_DUAL)
    {
        if(tmds_port[i].port_power == MsCombo_TMDS_POWER_ON) // Power On
        {
            if(mhal_tmds_DE_Stable(i))//DE stable
            {
                tmds_port[i].InputVideoMode = mhal_tmds_GetInputType(i);
                if(tmds_port[i].InputVideoMode == 0) //DVI mode
                {
                    if(mhal_tmds_TimingChg(i))
                    {
                        TMDS_PrtMsg("Combo Timing CHG");
						tmds_port[i].input_color.ucColorType = COMBO_COLOR_FORMAT_RGB;
                        if(mhal_tmds_IsDualLink(i))
                        {
                            TMDS_PrtMsg("Dual");
                            mhal_tmds_DualLink_Setting(i, TRUE);

                        }
                        else
                        {
                            TMDS_PrtMsg("Single");
                            mhal_tmds_DualLink_Setting(i, FALSE);

                        }
                        mhal_tmds_reset_status(i, TRUE, 0xDD);
                    }
                }
                else
                {
                    TMDS_PrtMsg("not support HDMI mode in DVI port!!!"); //temp
                }
            }
            else  //DE unstable
            {
                //do nothing.
            }
        }
    }
    else if(tmds_port[i].port_type == MsCombo_TMDS_TYPE_DVI)
    {
        if(tmds_port[i].port_power == MsCombo_TMDS_POWER_ON) // Power On
        {
            if(mhal_tmds_DE_Stable(i))//DE stable
            {
                tmds_port[i].InputVideoMode = mhal_tmds_GetInputType(i);
                if(tmds_port[i].InputVideoMode == 0) //DVI mode
                {
                    if(mhal_tmds_TimingChg(i))
                    {
                        TMDS_PrtMsg("Combo Timing CHG");
						tmds_port[i].input_color.ucColorType = COMBO_COLOR_FORMAT_RGB;
                        mhal_tmds_reset_status(i, TRUE, 0xDD);
                    }
                }
                else
                {
                    TMDS_PrtMsg("not support HDMI mode in DVI port!!!"); //temp
                }
            }
            else  //DE unstable
            {
                //do nothing.
            }
        }
    }

    if(i < MSCombo_TMDS_PORT_MAX)
        i++;
    else
        i = MSCombo_TMDS_PORT_0;
}

void mdrv_tmds_TimerInterrupt(void)
{
    WORD i;

    for(i = MSCombo_TMDS_PORT_0; i <= MSCombo_TMDS_PORT_MAX; i++)
    {
        if(tmds_port[i].port_power == MsCombo_TMDS_POWER_ON)
        {
            if(tmds_port[i].AVI_PktLossCnt <= AviPktLossBound )
                tmds_port[i].AVI_PktLossCnt++;

            if(tmds_port[i].VS_PktLossCnt <= VS_PKT_LOSS_BOUND)
                tmds_port[i].VS_PktLossCnt++;
        }
    }
}

void mdrv_tmds_Set_HPD(BYTE ucComboSelect, MS_BOOL bSetHPD)
{
    mhal_tmds_Set_HPD(ucComboSelect, bSetHPD);
}

ST_COMBO_COLOR_FORMAT mdrv_tmds_Get_Color_Format(BYTE ucInputPort)
{
    ST_COMBO_COLOR_FORMAT tmpColorFmt = {COMBO_COLOR_FORMAT_DEFAULT, COMBO_COLOR_RANGE_DEFAULT, COMBO_COLORIMETRY_NONE};

    if(ucInputPort <= MSCombo_TMDS_PORT_MAX)
        return tmds_port[ucInputPort].input_color;
    else
        return tmpColorFmt;
}


BYTE mdrv_tmds_Input_Is_HDMI(BYTE ucInputPort)
{
    if(ucInputPort <= MSCombo_TMDS_PORT_MAX)
        return tmds_port[ucInputPort].InputVideoMode;
    else
        return 0;
}

void mdrv_tmds_GetPacketContent(BYTE ucInputPort, EN_PACKET_DEFINE_TYPE ucPacketType, BYTE ucPacketLength, BYTE *pPacketData)
{
    //return all zero data if ucInputPort value is invalid
    BYTE i;

    for (i=0; i < ucPacketLength; i++)
    {
        *(pPacketData + i) = 0;
    }

    if(ucInputPort <= MSCombo_TMDS_PORT_MAX)
    {
        WORD wPktStatus = 0;
        BOOL bPktReceived = FALSE;

        wPktStatus = tmds_port[ucInputPort].InputPacketStatus;

        //no packet received
        if (wPktStatus == 0)
            return;
        else
        {
            switch (ucPacketType)
            {
                case COMBO_PACKET_NULL:
                    if (wPktStatus & _BIT8)
                        bPktReceived = TRUE;
                break;
                case COMBO_PACKET_AUDIO_CLOCK_REGENERATION:
                    if (wPktStatus & _BIT6)
                        bPktReceived = TRUE;
                break;
                case COMBO_PACKET_AUDIO_SAMPLE:
                    if (wPktStatus & _BIT5)
                        bPktReceived = TRUE;
                break;
                case COMBO_PACKET_GENERAL_CONTROL:
                    if (wPktStatus & _BIT4)
                        bPktReceived = TRUE;
                break;
                case COMBO_PACKET_ACP_PACKET:
                    if (wPktStatus & _BIT11)
                        bPktReceived = TRUE;
                break;
                case COMBO_PACKET_ISRC1_PACKET:
                    if (wPktStatus & _BIT10)
                        bPktReceived = TRUE;
                break;
                case COMBO_PACKET_ISRC2_PACKET:
                    if (wPktStatus & _BIT9)
                        bPktReceived = TRUE;
                break;
                case COMBO_PACKET_ONE_BIT_AUDIO_SAMPLE:
                    //TBD: not support?
                break;
                case COMBO_PACKET_DST_AUDIO:
                    if (wPktStatus & _BIT12)
                        bPktReceived = TRUE;
                break;
                case COMBO_PACKET_HBR_AUDIO_STREAM:
                    if (wPktStatus & _BIT15)
                        bPktReceived = TRUE;
                break;
                case COMBO_PACKET_GAMUT_METADATA:
                    if (wPktStatus & _BIT14)
                        bPktReceived = TRUE;
                break;
                case COMBO_PACKET_VS_INFOFRAME:
                    if (wPktStatus & _BIT7)
                        bPktReceived = TRUE;
                break;
                case COMBO_PACKET_AVI_INFOFRAME:
                    if (wPktStatus & _BIT3)
                        bPktReceived = TRUE;
                break;
                case COMBO_PACKET_SPD_INFOFRAME:
                    if (wPktStatus & _BIT2)
                        bPktReceived = TRUE;
                break;
                case COMBO_PACKET_AUD_INFOFRAME:
                    if (wPktStatus & _BIT1)
                        bPktReceived = TRUE;
                break;
                case COMBO_PACKET_MJPGSRC_INFOFRAME:
                    if (wPktStatus & _BIT0)
                        bPktReceived = TRUE;
                break;
                case COMBO_PACKET_DP_MISC:
                default:
                    //not support here
                break;
            }

            if (bPktReceived == TRUE)
                mhal_tmds_GetPacketContent(ucInputPort, ucPacketType, ucPacketLength, pPacketData);
            else
                return;
        }
    }
}
#endif

#if ENABLE_SYNC_CHECK_AGAIN
void mdrv_tmds_LoadHDCPKey(BYTE ucPortSelect, BYTE ucBcaps, BYTE *pHDCPKey, BYTE *pBKSV)
{
    mhal_tmds_LoadHDCPKey(ucPortSelect, ucBcaps, pHDCPKey, pBKSV);
}

BYTE mdrv_tmds_GetDVIDualMainPort(BYTE ucPortSelect)
{
    return mhal_tmds_GetDVIDualMainPort(ucPortSelect);
}
Bool mdrv_tmds_LoadKeyViaEfuse(BYTE* HDCP_KEY_TABLE, BYTE* HDCP_BKSV, EFUSE_SEL eFuse)
{
    return mhal_tmds_LoadKeyViaEfuse(HDCP_KEY_TABLE, HDCP_BKSV, eFuse);
}
#endif
#if ENABLE_CABLE_DET
BYTE mdrv_tmds_DetectCablePlug(BYTE ucInputPort)
{
    return mhal_tmds_DetectCablePlug(ucInputPort);
}
#endif
#endif // _MDRV_TMDS_C_

