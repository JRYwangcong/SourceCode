#include <string.h>
#include "drvMCCSCal.h"
#include "menufunc.h"
#include "msACE.h"
#include "NVRam.h"
#include "msPQ.h"

#if (ENABLE_MSTV_UART_DEBUG)
#define LUT_printData(str, value)   printData(str, value)
#define LUT_printMsg(str)               printMsg(str)
#else
#define LUT_printData(str, value)
#define LUT_printMsg(str)
#endif

#if ENABLE_COLOR_CALIBRATION_FOR_THIRD_PARTY
DWORD _addrOfMCCSStruct = ADDR_THIRD_PARTY_BLOCK;
#endif
xdata struct  MCCS_PARAMETER  g_MCCS_parameter;

extern WORD _checkSum;
BYTE MCCSCalibrationHandler(BYTE CommandCode)
{
    switch(DDCBuffer[2])
    {
        case MCCS_COLORAP_Post_Gamma :
        case MCCS_COLORAP_Pre_Gamma :
        {
            //msLoadPostGammaFromSRam();
            BYTE * u8GammaTable ;
            if (DDCBuffer[2] ==MCCS_COLORAP_Post_Gamma )
                u8GammaTable = g_MCCS_parameter.u8PostGamma[0];
            else
                u8GammaTable = g_MCCS_parameter.u8PreGamma[0];

            WORD addr, idx;
            BYTE sum, cnt, ch;
            sum=0;
            ch = DDCBuffer[3] & 0x03 ;          // gamma channel
            cnt = DDCBuffer[4] ;                // data size
            addr = (WORD)(DDCBuffer[5] << 8 );
            addr += DDCBuffer[6];               // start addr-hi, lo
            if ( CommandCode == MCCS_COLORAP_COMMAND_WRITE)
            {
                if ((cnt > 60) || (addr+cnt > _maxEntrieBytes) || (ch > 3))
                    return 0;

                for(idx = 0; idx < cnt; idx+=2)
                {
                    sum ^=  DDCBuffer[7 + idx] ;
                    sum ^=  DDCBuffer[7 + idx + 1] ;
                    u8GammaTable[ch*_maxEntrieBytes + addr + idx] = DDCBuffer[7 + idx + 1];
                    u8GammaTable[ch*_maxEntrieBytes +addr + idx + 1] = DDCBuffer[7 + idx];
                }

                _checkSum = sum;

                if (ch ==2  && addr+cnt == _maxEntrieBytes)
                {
                    if (DDCBuffer[2] == MCCS_COLORAP_Post_Gamma)
                    {
                        g_MCCS_parameter.bPostGammaModified = 1;
                        //int color = 0;
                        BYTE *GammaTab[][3] =
                        {
                            {g_MCCS_parameter.u8PostGamma[0] , g_MCCS_parameter.u8PostGamma[1] , g_MCCS_parameter.u8PostGamma[2]},
                        };
                        //for( color = 0 ;  color < 3 ; ++color)
                        //    msDrvGammaWriteTbl_1024E_14B(USER_PREF_WIN_SEL, color, GammaTab[0]);
                        msAPI_GammaLoadTbl_1024E_14B(USER_PREF_WIN_SEL, GammaTab[0]);
                        msAPI_GammaEnable(USER_PREF_WIN_SEL, true);
                    }
                    else
                    {
                        g_MCCS_parameter.bPreGammaModified = 1;
                        float fDeGmma[_channelNum][_maxEntries];
                        int color = 0;
                        for (color = 0; color < _channelNum; color++)
                        {
                            for (idx = 0; idx < _maxEntries; idx++)
                            {
                                fDeGmma[color][idx] = (u8GammaTable[color*_maxEntrieBytes + idx * 2] | (u8GammaTable[color*_maxEntrieBytes + idx * 2 + 1] << 8)) /4.0;
                            }
                        }
                    #if (CHIP_ID==MST9U3) //sam 141203
                        WORD wDeGmma[_channelNum][256];
                        for (color = 0; color < _channelNum; color++)
                        {
                            for (idx = 0; idx < 256; idx++)
                        {
                                wDeGmma[color][idx] = fDeGmma[color][idx*4];
                            }
                        }
                        WORD *DeGammaTab[][3] =
                        {
                            {wDeGmma[0] , wDeGmma[1] , wDeGmma[2]},
                        };
                        msAPI_DeGammaLoadTbl_256E_12B(USER_PREF_WIN_SEL, DeGammaTab[0]);
                    #else
                        unsigned char compressedGma[_channelNum][_comMaxEntries];
                        unsigned char segOft[_segPntNum];
                        unsigned int segPnt[_channelNum][_segPntNum];
                        msCompressGamma(fDeGmma, compressedGma, segOft, segPnt);
                        BYTE *DeGammaTab[][4] =
                        {
                            {compressedGma[0] , compressedGma[1] , compressedGma[2], segOft},
                        };
                        WORD wSegPnt[_channelNum*_segPntNum];
                        for (color = 0; color < _channelNum; color++)
                        {
                            for (idx = 0; idx < _segPntNum; idx++)
                            {
                                wSegPnt[color*_segPntNum+idx] = segPnt[color][idx];
                            }
                        }
                        WORD *segPntTab[][1] =
                        {
                            {wSegPnt},
                        };
                        msAPI_DeGammaLoadTbl_Compressed(USER_PREF_WIN_SEL, DeGammaTab[0], segPntTab[0]);
                    #endif
                        msAPI_DeGammaEnable( USER_PREF_WIN_SEL, true);
                    }

                }
            }
            else
            {
                if ((cnt > 60) || (addr+cnt > _maxEntrieBytes) || (ch > 3))
                    return 0;

                DDCBuffer[0] = cnt;
                for(idx = 1; idx <= cnt; idx+=2)
                {
                    DDCBuffer[idx] = u8GammaTable[ch*_maxEntrieBytes + addr + idx];
                    DDCBuffer[idx + 1] = u8GammaTable[ch*_maxEntrieBytes + addr + idx - 1];
                    sum ^=  DDCBuffer[idx] ;
                    sum ^=  DDCBuffer[idx + 1] ;
                }
                _checkSum = sum;
                return cnt;
            }

        }
        break;
        case MCCS_COLORAP_Fix_Gamma:
        {
            WORD addr, idx;
            BYTE sum, cnt, ch;
            sum=0;
            ch = DDCBuffer[3] & 0x03 ;          // gamma channel
            cnt = DDCBuffer[4] ;                // data size
            addr = (WORD)(DDCBuffer[5] << 8 );
            addr += DDCBuffer[6];               // start addr-hi, lo

            if ( CommandCode == MCCS_COLORAP_COMMAND_WRITE)
            {
                if ((cnt > 60) || (addr+cnt > 512) || (ch > 3))
                    return 0;

                g_MCCS_parameter.bFixGammaModified = 1;
                for(idx = 0; idx < cnt; idx+=2)
                {
                    sum ^=  DDCBuffer[7 + idx] ;
                    sum ^=  DDCBuffer[7 + idx + 1] ;
                    g_MCCS_parameter.u8FixGamma[ch][addr + idx] = DDCBuffer[7 + idx + 1];
                    g_MCCS_parameter.u8FixGamma[ch][addr + idx + 1] = DDCBuffer[7 + idx];
                }
                _checkSum = sum;

                if (ch ==2  && addr+cnt == 512)
                {
                    BYTE *FixGammaTab[][3] =
                    {
                        {g_MCCS_parameter.u8FixGamma[0] , g_MCCS_parameter.u8FixGamma[1] , g_MCCS_parameter.u8FixGamma[2]},
                    };
                    msAPI_FixGammaLoadTbl_256E_16B(USER_PREF_WIN_SEL, FixGammaTab[0]);
                    msAPI_FixGammaEnable(USER_PREF_WIN_SEL, _ENABLE);
                }
            }
            else
            {
                if ((cnt > 60) || (addr+cnt > 512) || (ch > 3))
                    return 0;

                DDCBuffer[0] = cnt;
                for(idx = 1; idx <= cnt; idx+=2)
                {
                    DDCBuffer[idx] = g_MCCS_parameter.u8FixGamma[ch][addr + idx];
                    DDCBuffer[idx + 1] = g_MCCS_parameter.u8FixGamma[ch][addr + idx - 1];
                    sum ^=  DDCBuffer[idx] ;
                    sum ^=  DDCBuffer[idx + 1] ;
                }
                _checkSum = sum;
                return cnt;
            }
        }
        break;
        case MCCS_COLORAP_6axis_Hue :
        {
            if ( CommandCode == MCCS_COLORAP_COMMAND_WRITE)
            {
                g_MCCS_parameter.bSixAxisHueModified = 1;
                msAPI_CustomHueEnable(USER_PREF_WIN_SEL, true);
                g_MCCS_parameter.u8SixAxisHue[0] = DDCBuffer[3];
                g_MCCS_parameter.u8SixAxisHue[1] = DDCBuffer[4];
                g_MCCS_parameter.u8SixAxisHue[2] = DDCBuffer[5];
                g_MCCS_parameter.u8SixAxisHue[3] = DDCBuffer[6];
                g_MCCS_parameter.u8SixAxisHue[4] = DDCBuffer[7];
                g_MCCS_parameter.u8SixAxisHue[5] = DDCBuffer[8];
                msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(g_MCCS_parameter.u8SixAxisHue[0]), eIHC_R);
                msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(g_MCCS_parameter.u8SixAxisHue[1]), eIHC_G);
                msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(g_MCCS_parameter.u8SixAxisHue[2]), eIHC_B);
                msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(MAX_CUSTOM_HUE_VALUE-g_MCCS_parameter.u8SixAxisHue[3]), eIHC_C);
                msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(MAX_CUSTOM_HUE_VALUE-g_MCCS_parameter.u8SixAxisHue[4]), eIHC_M);
                msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(MAX_CUSTOM_HUE_VALUE-g_MCCS_parameter.u8SixAxisHue[5]), eIHC_Y);
           }
           else
           {
                DDCBuffer[0] = 0x06;
                DDCBuffer[1] = g_MCCS_parameter.u8SixAxisHue[0];
                DDCBuffer[2] = g_MCCS_parameter.u8SixAxisHue[1];
                DDCBuffer[3] = g_MCCS_parameter.u8SixAxisHue[2];
                DDCBuffer[4] = g_MCCS_parameter.u8SixAxisHue[3];
                DDCBuffer[5] = g_MCCS_parameter.u8SixAxisHue[4];
                DDCBuffer[6] = g_MCCS_parameter.u8SixAxisHue[5];
                return 6;
            }
        }
        break;
        case MCCS_COLORAP_6axis_Saturation :
        {
            if ( CommandCode == MCCS_COLORAP_COMMAND_WRITE)
            {
                g_MCCS_parameter.bSixAxisSatModified = 1;
                msAPI_CustomSaturationEnable(USER_PREF_WIN_SEL, true);
                g_MCCS_parameter.u8SixAxisSat[0] = DDCBuffer[3];
                g_MCCS_parameter.u8SixAxisSat[1] = DDCBuffer[4];
                g_MCCS_parameter.u8SixAxisSat[2] = DDCBuffer[5];
                g_MCCS_parameter.u8SixAxisSat[3] = DDCBuffer[6];
                g_MCCS_parameter.u8SixAxisSat[4] = DDCBuffer[7];
                g_MCCS_parameter.u8SixAxisSat[5] = DDCBuffer[8];
                msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(g_MCCS_parameter.u8SixAxisSat[0]), eICC_R);
                msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(g_MCCS_parameter.u8SixAxisSat[1]), eICC_G);
                msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(g_MCCS_parameter.u8SixAxisSat[2]), eICC_B);
                msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(g_MCCS_parameter.u8SixAxisSat[3]), eICC_C);
                msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(g_MCCS_parameter.u8SixAxisSat[4]), eICC_M);
                msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(g_MCCS_parameter.u8SixAxisSat[5]), eICC_Y);
           }
           else
           {
                DDCBuffer[0] = 0x06;
                DDCBuffer[1] = g_MCCS_parameter.u8SixAxisSat[0];
                DDCBuffer[2] = g_MCCS_parameter.u8SixAxisSat[1];
                DDCBuffer[3] = g_MCCS_parameter.u8SixAxisSat[2];
                DDCBuffer[4] = g_MCCS_parameter.u8SixAxisSat[3];
                DDCBuffer[5] = g_MCCS_parameter.u8SixAxisSat[4];
                DDCBuffer[6] = g_MCCS_parameter.u8SixAxisSat[5];
                return 6;
            }
        }
        break;
        case MCCS_COLORAP_6axis_Brightness:
        {
            if ( CommandCode == MCCS_COLORAP_COMMAND_WRITE)
            {
                g_MCCS_parameter.bSixAxisBriModified = 1;
                msAPI_CustomBrightnessEnable(USER_PREF_WIN_SEL, true);
                int idx = 0;
                for (idx = 0; idx < 6; idx++)
                {
                    g_MCCS_parameter.u8SixAxisBri[idx] = DDCBuffer[idx+3];
                }
                for (idx = 0; idx < 6; idx++)
                {
                    msAPI_AdjustCustomBrightness(USER_PREF_WIN_SEL, g_MCCS_parameter.u8SixAxisBri[idx], idx + 1);
                }
            }
            else
            {
                DDCBuffer[0] = 0x06;
                DDCBuffer[1] = g_MCCS_parameter.u8SixAxisBri[0];
                DDCBuffer[2] = g_MCCS_parameter.u8SixAxisBri[1];
                DDCBuffer[3] = g_MCCS_parameter.u8SixAxisBri[2];
                DDCBuffer[4] = g_MCCS_parameter.u8SixAxisBri[3];
                DDCBuffer[5] = g_MCCS_parameter.u8SixAxisBri[4];
                DDCBuffer[6] = g_MCCS_parameter.u8SixAxisBri[5];
                return 6;
            }
        }
        break;
        case MCCS_COLORAP_Brightness:
        {
            if ( CommandCode == MCCS_COLORAP_COMMAND_WRITE)
            {
                g_MCCS_parameter.bBrightnessModified = 1;
                g_MCCS_parameter.u8Brightness = DDCBuffer[4];
                USER_PREF_BRIGHTNESS = DDCBuffer[4];   //Kojona150812 for Fix abnormity of setting brightness
                msAPI_AdjustBrightness(USER_PREF_BRIGHTNESS);
            }
            else
            {
                DDCBuffer[0] = 0x02;
                DDCBuffer[1] = 0x00;
                DDCBuffer[2] = g_MCCS_parameter.u8Brightness;
                return 2;
            }
        }
        break;
        case MCCS_COLORAP_Contrast:
        {
            if ( CommandCode == MCCS_COLORAP_COMMAND_WRITE)
            {
                g_MCCS_parameter.bContrastModified = 1;
                g_MCCS_parameter.u8Contrast = DDCBuffer[4];
                msAPI_AdjustContrast(USER_PREF_WIN_SEL , g_MCCS_parameter.u8Contrast);
            }
            else
            {
                DDCBuffer[0] = 0x02;
                DDCBuffer[1] = 0x00;
                DDCBuffer[2] = g_MCCS_parameter.u8Contrast;
                return 2;
            }
        }
        break;
        case MCCS_COLORAP_Saturation:
        {
            if ( CommandCode == MCCS_COLORAP_COMMAND_WRITE)
            {
                g_MCCS_parameter.bSaturationModified = 1;
                g_MCCS_parameter.u8Saturation = DDCBuffer[4];
                msAPI_AdjustSaturation(USER_PREF_WIN_SEL, g_MCCS_parameter.u8Saturation);
            }
            else
            {
                DDCBuffer[0] = 0x02;
                DDCBuffer[1] = 0x00;
                DDCBuffer[2] = g_MCCS_parameter.u8Saturation;
                return 2;
            }
        }
        break;
        case MCCS_COLORAP_Hue:
        {
            if ( CommandCode == MCCS_COLORAP_COMMAND_WRITE)
            {
                g_MCCS_parameter.bHueModified= 1;
                g_MCCS_parameter.u8Hue = DDCBuffer[4];
                msAPI_AdjustVideoHue(USER_PREF_WIN_SEL, GetProperHue(g_MCCS_parameter.u8Hue));
            }
            else
            {
                DDCBuffer[0] = 0x02;
                DDCBuffer[1] = 0x00;
                DDCBuffer[2] = g_MCCS_parameter.u8Hue;
                return 2;
            }
        }
        break;
        case MCCS_COLORAP_RGB_Gain:
        {
            if ( CommandCode == MCCS_COLORAP_COMMAND_WRITE)
            {
                 g_MCCS_parameter.bRGBGainModified= 1;
                 g_MCCS_parameter.u8RGBGain[0] = DDCBuffer[3];
                 g_MCCS_parameter.u8RGBGain[1] = DDCBuffer[4];
                 g_MCCS_parameter.u8RGBGain[2] = DDCBuffer[5];
                 msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, g_MCCS_parameter.u8RGBGain[0], g_MCCS_parameter.u8RGBGain[1], g_MCCS_parameter.u8RGBGain[2]);
            }
            else
            {
                DDCBuffer[0] = 0x03;
                DDCBuffer[1] = g_MCCS_parameter.u8RGBGain[0];
                DDCBuffer[2] = g_MCCS_parameter.u8RGBGain[1];
                DDCBuffer[3] = g_MCCS_parameter.u8RGBGain[2];
                return 3;
            }
        }
        break;
        case MCCS_COLORAP_RGB_Offset:
        {
            if ( CommandCode == MCCS_COLORAP_COMMAND_WRITE)
            {
                 g_MCCS_parameter.bRGBOffsetModified= 1;
                 g_MCCS_parameter.u8RGBOffset[0] = DDCBuffer[3];
                 g_MCCS_parameter.u8RGBOffset[1] = DDCBuffer[4];
                 g_MCCS_parameter.u8RGBOffset[2] = DDCBuffer[5];
                msAPI_AdjustBlacklevel(USER_PREF_WIN_SEL, g_MCCS_parameter.u8RGBOffset[0], g_MCCS_parameter.u8RGBOffset[1], g_MCCS_parameter.u8RGBOffset[2]);
            }
            else
            {
                DDCBuffer[0] = 0x03;
                DDCBuffer[1] = g_MCCS_parameter.u8RGBOffset[0];
                DDCBuffer[2] = g_MCCS_parameter.u8RGBOffset[1];
                DDCBuffer[3] = g_MCCS_parameter.u8RGBOffset[2];
                return 3;
            }
        }
        break;
        case MCCS_COLORAP_Color_Matrix:
        {
            if ( CommandCode == MCCS_COLORAP_COMMAND_WRITE)
            {
                g_MCCS_parameter.bColorMatrixModified= 1;
                int idx = 0;
                for (idx = 0; idx < 9; idx++)
                {
                     g_MCCS_parameter.ColorMatrix[idx] = (DDCBuffer[3+idx*2] << 8) | (DDCBuffer[4+idx*2]);
                }
                short* psMatrix = &g_MCCS_parameter.ColorMatrix[0];
                short val = 0;
                // 2' complement  format transfer to short data format;
                for (idx = 0; idx < 9; idx++)
                {
                    val = psMatrix[idx];
                    //LUT_printData("val -before %d", val);
                    if ((val & _BIT12) == _BIT12)
                    {
                        val &= 0xFFF;
                        val = 0x1000 - val;
                        val *= -1;
                    }
                    psMatrix[idx] = val;
                    //LUT_printData("val -after %d", val);
                }
                msWritexvYccColorMatrix(USER_PREF_WIN_SEL, psMatrix);
                msEnablexvYccColorMatrix( USER_PREF_WIN_SEL, true);
            }
            else
            {
                DDCBuffer[0] = 0x12;
                int idx = 0;
                for (idx = 0; idx < 9; idx++)
                {
                    DDCBuffer[2 * idx + 1] =  (g_MCCS_parameter.ColorMatrix[idx] >> 8) & 0xFF;
                    DDCBuffer[2 * (idx + 1)] =  g_MCCS_parameter.ColorMatrix[idx] & 0xFF;
                }
                return 18;
            }
        }
        break;
        case MCCS_COLORAP_Gamma_CheckSum:
        {
            DDCBuffer[2] = _checkSum >>8;
            DDCBuffer[1] = _checkSum & 0xFF;
            DDCBuffer[0] = 2;
            return 2;
        }
        break;
        case MCCS_COLORAP_ResetPostGamma:
        {
            msAPI_GammaEnable(USER_PREF_WIN_SEL, false);


            g_MCCS_parameter.bPostGammaModified = 1;
            int idx = 0;
            WORD GammaValue = 0x0010; //8.6 format

            for (idx = 0; idx < _maxEntries; idx++)
            {
               g_MCCS_parameter.u8PostGamma[0][2*idx] = g_MCCS_parameter.u8PostGamma[1][2*idx] = g_MCCS_parameter.u8PostGamma[2][2*idx] = (GammaValue * idx) & 0xFF;
               g_MCCS_parameter.u8PostGamma[0][2*idx+1] = g_MCCS_parameter.u8PostGamma[1][2*idx+1] = g_MCCS_parameter.u8PostGamma[2][2*idx+1] = ((GammaValue * idx) >> 8) & 0xFF;
            }

            BYTE *GammaTab[][3] =
            {
            {g_MCCS_parameter.u8PostGamma[0] , g_MCCS_parameter.u8PostGamma[1] , g_MCCS_parameter.u8PostGamma[2],},
            };
            //int color = 0;
            //for( color = 0 ;  color < 3 ; ++color)
            //    msDrvGammaWriteTbl_1024E_14B(USER_PREF_WIN_SEL, color, GammaTab[0]);
            msAPI_GammaLoadTbl_1024E_14B(USER_PREF_WIN_SEL, GammaTab[0]);
            msAPI_GammaEnable(USER_PREF_WIN_SEL, true);
        }
        break;
        case MCCS_COLORAP_ResetFixGamma:
        {
            msAPI_FixGammaEnable(USER_PREF_WIN_SEL, _DISABLE);

            g_MCCS_parameter.bFixGammaModified = 1;
            int idx = 0;
            WORD GammaValue = 0x0010; //8.4 format

            for (idx = 0; idx < 256; idx++)
            {
               g_MCCS_parameter.u8FixGamma[0][2*idx] = g_MCCS_parameter.u8FixGamma[1][2*idx] = g_MCCS_parameter.u8FixGamma[2][2*idx] = (GammaValue * idx) & 0xFF;
               g_MCCS_parameter.u8FixGamma[0][2*idx+1] = g_MCCS_parameter.u8FixGamma[1][2*idx+1] = g_MCCS_parameter.u8FixGamma[2][2*idx+1] = ((GammaValue * idx) >> 8) & 0xFF;
            }

            BYTE *FixGammaTab[][3] =
            {
                {g_MCCS_parameter.u8FixGamma[0] , g_MCCS_parameter.u8FixGamma[1] , g_MCCS_parameter.u8FixGamma[2]},
            };
            msAPI_FixGammaLoadTbl_256E_16B(USER_PREF_WIN_SEL, FixGammaTab[0]);

            msAPI_FixGammaEnable(USER_PREF_WIN_SEL, _ENABLE);
        }
        break;
        case MCCS_COLORAP_ResetPreGamma:
        {
            msAPI_DeGammaEnable( USER_PREF_WIN_SEL, false);


            int idx = 0;
            WORD GammaValue = 0x0010; //8.6 format
            int color = 0;

            g_MCCS_parameter.bPreGammaModified = 1;

            for (idx = 0; idx < _maxEntries; idx++)
            {
               g_MCCS_parameter.u8PreGamma[0][2*idx] = g_MCCS_parameter.u8PreGamma[1][2*idx] = g_MCCS_parameter.u8PreGamma[2][2*idx] = (GammaValue * idx) & 0xFF;
               g_MCCS_parameter.u8PreGamma[0][2*idx+1] = g_MCCS_parameter.u8PreGamma[1][2*idx+1] = g_MCCS_parameter.u8PreGamma[2][2*idx+1] = ((GammaValue * idx) >> 8) & 0xFF;
            }

            float fDeGmma[_channelNum][_maxEntries];
            BYTE LowByte=0, HiByte=0;

            for (color = 0; color < _channelNum; color++)
            {
                for (idx = 0; idx < _maxEntries; idx++)
                {
                    LowByte = g_MCCS_parameter.u8PreGamma[color][idx * 2];
                    HiByte = g_MCCS_parameter.u8PreGamma[color][idx * 2 + 1];
                    fDeGmma[color][idx] = (LowByte | (HiByte << 8))/4.0;
                }
            }
        #if (CHIP_ID==MST9U3) //sam 141203
            WORD wDeGmma[_channelNum][256];
            for (color = 0; color < _channelNum; color++)
            {
                for (idx = 0; idx < _maxEntries; idx++)
            {
                    wDeGmma[color][idx] = fDeGmma[color][idx*4];
                }
            }
            WORD *DeGammaTab[][3] =
            {
                {wDeGmma[0] , wDeGmma[1] , wDeGmma[2]},
            };
            msAPI_DeGammaLoadTbl_256E_12B(USER_PREF_WIN_SEL, DeGammaTab[0]);
        #else
            unsigned char compressedGma[_channelNum][_comMaxEntries];
            unsigned char segOft[_segPntNum];
            unsigned int segPnt[_channelNum][_segPntNum];
            msCompressGamma(fDeGmma, compressedGma, segOft, segPnt);
            BYTE *DeGammaTab[][4] =
            {
                {compressedGma[0] , compressedGma[1] , compressedGma[2], segOft},
            };
            WORD wSegPnt[_channelNum*_segPntNum];
            for (color = 0; color < _channelNum; color++)
            {
                for (idx = 0; idx < _segPntNum; idx++)
                {
                    wSegPnt[color*_segPntNum+idx] = segPnt[color][idx];
                }
            }
            WORD *segPntTab[][1] =
            {
                {wSegPnt},
            };
            msAPI_DeGammaLoadTbl_Compressed(USER_PREF_WIN_SEL, DeGammaTab[0], segPntTab[0]);
        #endif
            msAPI_DeGammaEnable( USER_PREF_WIN_SEL, true);
        }
        break;
        case MCCS_COLORAP_RestoreFactory:
        {
            ResetColorSettingAll();
            //msDiscardMCCSColorData();  //need check again.
        }
        break;

        case MCCS_COLORAP_Color_Preset:
        {
            //msPQ_BypassPQ(USER_PREF_WIN_SEL, TRUE);
            memset(&g_MCCS_parameter, 0, sizeof(g_MCCS_parameter));
            g_MCCS_parameter.u8PresetNumber = DDCBuffer[3];
            msBypassMCCSColorEngine();
            #if ENABLE_SaveMCCSData
                if (msLoadMCCSColorData(g_MCCS_parameter.u8PresetNumber))
                {
                    msApplyMCCSColorData();
                }
            #endif
        }
        break;

        case MCCS_COLORAP_SaveAdjustment:
        {
            #if ENABLE_SaveMCCSData
                msSaveMCCSColorData(g_MCCS_parameter.u8PresetNumber);
            #endif
        }
        break;
        case MCCS_COLORAP_DisableOSD:
        {
        }
        break;
        case MCCS_COLORAP_EnableOSD:
        {
        }
        break;

        case MCCS_COLORAP_ToggleInternalPattern:
        {
            if ( DDCBuffer[3] == 0x00 )
            {
                msSetTestPattern(FALSE, 0, 0, 0);
            }
            else
            {
                //On
	    	        msSetTestPattern(TRUE, 0, 0, 0);
            }
        }
        break;

        case MCCS_COLORAP_InternalPattern:
        {
            msSetTestPattern(TRUE, DDCBuffer[3], DDCBuffer[4], DDCBuffer[5]);
        }
        break;

        case MCCS_COLORAP_ToggleXVYCCPATTERN :
        {
            if ( DDCBuffer[3] == 0x00 )
            {
                //Off
                if (msAPIWinColorIsYUV(USER_PREF_WIN_SEL))
                {
                    msEnableColorMatrix(USER_PREF_WIN_SEL, TRUE, FALSE);
                }
                msSetColorEngineTestPattern(USER_PREF_WIN_SEL, _DISABLE, 0x00, 0x00, 0x00);
            }
            else
            {
                //On
                if (msAPIWinColorIsYUV(USER_PREF_WIN_SEL))
                {
                    msEnableColorMatrix(USER_PREF_WIN_SEL, FALSE, FALSE);
                }
                msSetColorEngineTestPattern(USER_PREF_WIN_SEL, _ENABLE, 0x00, 0x00, 0x00);
            }
        }
        break;

        case MCCS_COLORAP_XVYCCPATTERN :
        {
            WORD wR = DDCBuffer[3]*0x3FF/0xFF;
            WORD wG = DDCBuffer[4]*0x3FF/0xFF;
            WORD wB = DDCBuffer[5]*0x3FF/0xFF;
            msSetColorEngineTestPattern(USER_PREF_WIN_SEL, _ENABLE, wR, wG, wB);
        }
        break;

        case MCCS_COLORAP_Bypass:
        {
            //msPQ_BypassPQ(USER_PREF_WIN_SEL, TRUE);
            //msSetColorEngine(FALSE);
            msBypassMCCSColorEngine();
        }
        break;

     }
     return 0;

}

void msBypassMCCSColorEngine(void)
{
    msAPI_GammaEnable(USER_PREF_WIN_SEL, _DISABLE);
    msAPI_FixGammaEnable(USER_PREF_WIN_SEL, _DISABLE);
    msAPI_DeGammaEnable( USER_PREF_WIN_SEL, _DISABLE);
    msAPI_CustomHueEnable(USER_PREF_WIN_SEL, _DISABLE);
    msAPI_CustomSaturationEnable(USER_PREF_WIN_SEL, _DISABLE);
    //msAPI_CustomBrightnessEnable(USER_PREF_WIN_SEL, true);

    msDrvBlackLevelEnable(USER_PREF_WIN_SEL,_DISABLE);
    msEnablexvYccColorMatrix( USER_PREF_WIN_SEL, _DISABLE);

}


#if ENABLE_SaveMCCSData
DWORD GetAddrOfMCCSStruct(BYTE CSIndex)
{
    return _addrOfMCCSStruct + sizeof(g_MCCS_parameter) * CSIndex;
}
//---------------------------------------------------------------------------
// Load MCCS struct from EEPROM
//---------------------------------------------------------------------------
BOOL msLoadMCCSColorData(BYTE CSIndex)
{
    LUT_printData("msLoadMCCSColorData CSIndex = %X", CSIndex);
    if (CSIndex >= NUM_OF_USER_CALIBRATION_BLOCK)
        return FALSE;

    DWORD addr = GetAddrOfMCCSStruct(CSIndex);
    LUT_printData("msLoadMCCSColorData addr : %d", addr);

    BYTE usModifiedFlag[NumOf3rdPartyColorEngine+1] = {0};
    {
        NVRam_ReadTbl(addr, (BYTE*)(&usModifiedFlag), sizeof(usModifiedFlag));

        int idx = 0;
        BYTE checksum = 0;
        for (idx = 0; idx < NumOf3rdPartyColorEngine; idx++)
        {
            if (1 == usModifiedFlag[idx])
                checksum += 1;
        }
        if (checksum != usModifiedFlag[NumOf3rdPartyColorEngine] || usModifiedFlag[NumOf3rdPartyColorEngine] == 0)
            return FALSE;
    }
    LUT_printMsg("msLoadMCCSColorData...");
    NVRam_ReadTbl(addr, (BYTE*)(&g_MCCS_parameter), sizeof(g_MCCS_parameter));
    return TRUE;
    //msApplyMCCSColorData();
}
//---------------------------------------------------------------------------
// Save MCCS struct to EEPROM
//---------------------------------------------------------------------------
void  msSaveMCCSColorData(BYTE CSIndex)
{
    DWORD addr = GetAddrOfMCCSStruct(CSIndex);
    BYTE *pFlag = (BYTE*)(&g_MCCS_parameter);
    int idx = 0;
    g_MCCS_parameter.u8ModifiedFlagCheckSum = 0;
    for (idx = 0; idx < NumOf3rdPartyColorEngine; idx++)
    {
        if (1 == pFlag[idx])
            g_MCCS_parameter.u8ModifiedFlagCheckSum += 1 ;
    }
    NVRam_WriteTbl(addr, (BYTE*)(&g_MCCS_parameter), sizeof(g_MCCS_parameter));
}

void  msDiscardMCCSColorData(BYTE idx)
{
    if (idx >= NUM_OF_USER_CALIBRATION_BLOCK)
        return;

    BYTE usDiscardModifiedFlag[NumOf3rdPartyColorEngine+1] = {0};
    DWORD addr = GetAddrOfMCCSStruct(idx);
    NVRam_WriteTbl(addr, (BYTE*)(&usDiscardModifiedFlag), sizeof(usDiscardModifiedFlag));
}

void msApplyMCCSColorData(void)
{
    if (1 == g_MCCS_parameter.bPostGammaModified)
    {
        //int color = 0;
        BYTE *GammaTab[][3] =
        {
            {g_MCCS_parameter.u8PostGamma[0] , g_MCCS_parameter.u8PostGamma[1] , g_MCCS_parameter.u8PostGamma[2],},
        };
        //for( color = 0 ;  color < 3 ; ++color)
            //msDrvGammaWriteTbl_1024E_14B(USER_PREF_WIN_SEL, color, GammaTab[0]);
        msAPI_GammaLoadTbl_1024E_14B(USER_PREF_WIN_SEL, GammaTab[0]);
        msAPI_GammaEnable(USER_PREF_WIN_SEL, true);
    }

    if (1 == g_MCCS_parameter.bFixGammaModified)
    {
        BYTE *FixGammaTab[][3] =
        {
            {g_MCCS_parameter.u8FixGamma[0] , g_MCCS_parameter.u8FixGamma[1] , g_MCCS_parameter.u8FixGamma[2]},
        };
        msAPI_FixGammaLoadTbl_256E_16B(USER_PREF_WIN_SEL, FixGammaTab[0]);
        msAPI_FixGammaEnable(USER_PREF_WIN_SEL, _ENABLE);
    }

    if (1 == g_MCCS_parameter.bPreGammaModified)
    {
        BYTE * u8GammaTable ;
        u8GammaTable = g_MCCS_parameter.u8PreGamma[0];
        float fDeGmma[_channelNum][_maxEntries];
        int color = 0;
        int idx = 0;
        for (color = 0; color < _channelNum; color++)
        {
            for (idx = 0; idx < _maxEntries; idx++)
            {
                 fDeGmma[color][idx] = (u8GammaTable[color*_maxEntrieBytes + idx * 2] | (u8GammaTable[color*_maxEntrieBytes + idx * 2 + 1] << 8))/4.0;
            }
        }
        #if (CHIP_ID==MST9U3)
            WORD wDeGmma[_channelNum][256];
            for (color = 0; color < _channelNum; color++)
            {
                for (idx = 0; idx < _maxEntries; idx++)
            {
                    wDeGmma[color][idx] = fDeGmma[color][idx*4];
                }
            }
            WORD *DeGammaTab[][3] =
            {
                {wDeGmma[0] , wDeGmma[1] , wDeGmma[2]},
            };
            msAPI_DeGammaLoadTbl_256E_12B(USER_PREF_WIN_SEL, DeGammaTab[0]);
        #else

            unsigned char compressedGma[_channelNum][_comMaxEntries];
            unsigned char segOft[_segPntNum];
            unsigned int segPnt[_channelNum][_segPntNum];

            msCompressGamma(fDeGmma, compressedGma, segOft, segPnt);
            BYTE *DeGammaTab[][4] =
            {
            {compressedGma[0] , compressedGma[1] , compressedGma[2],  segOft},
            };
            WORD wSegPnt[_channelNum*_segPntNum];
            for (color = 0; color < _channelNum; color++)
            {
                for (idx = 0; idx < _segPntNum; idx++)
                {
                    wSegPnt[color*_segPntNum+idx] = segPnt[color][idx];
                }
            }
            WORD *segPntTab[][1] =
            {
                {wSegPnt},
            };

            msAPI_DeGammaLoadTbl_Compressed(USER_PREF_WIN_SEL, DeGammaTab[0], segPntTab[0]);
        #endif
        msAPI_DeGammaEnable( USER_PREF_WIN_SEL, true);
    }

    if (1 == g_MCCS_parameter.bSixAxisHueModified)
    {
        msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(g_MCCS_parameter.u8SixAxisHue[0]), eIHC_R);
        msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(g_MCCS_parameter.u8SixAxisHue[1]), eIHC_G);
        msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(g_MCCS_parameter.u8SixAxisHue[2]), eIHC_B);
        msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(MAX_CUSTOM_HUE_VALUE-g_MCCS_parameter.u8SixAxisHue[3]), eIHC_C);
        msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(MAX_CUSTOM_HUE_VALUE-g_MCCS_parameter.u8SixAxisHue[4]), eIHC_M);
        msAPI_AdjustCustomHue(USER_PREF_WIN_SEL , GetRealIhcValue(MAX_CUSTOM_HUE_VALUE-g_MCCS_parameter.u8SixAxisHue[5]), eIHC_Y);
        msAPI_CustomHueEnable(USER_PREF_WIN_SEL, true);
    }

    if (1 == g_MCCS_parameter.bSixAxisSatModified)
    {
        msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(g_MCCS_parameter.u8SixAxisSat[0]), eICC_R);
        msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(g_MCCS_parameter.u8SixAxisSat[1]), eICC_G);
        msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(g_MCCS_parameter.u8SixAxisSat[2]), eICC_B);
        msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(g_MCCS_parameter.u8SixAxisSat[3]), eICC_C);
        msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(g_MCCS_parameter.u8SixAxisSat[4]), eICC_M);
        msAPI_AdjustCustomSaturation(USER_PREF_WIN_SEL , GetRealICCValue(g_MCCS_parameter.u8SixAxisSat[5]), eICC_Y);
        msAPI_CustomSaturationEnable(USER_PREF_WIN_SEL, true);
    }

    if (1 == g_MCCS_parameter.bSixAxisBriModified)
    {
        int idx = 0;
        for (idx = 0; idx < 6; idx++)
        {
            msAPI_AdjustCustomBrightness(USER_PREF_WIN_SEL, g_MCCS_parameter.u8SixAxisBri[idx], idx + 1);
        }
        msAPI_CustomBrightnessEnable(USER_PREF_WIN_SEL, true);
    }

    if (1 == g_MCCS_parameter.bRGBGainModified)
    {
        msAPI_AdjustRGBColor(USER_PREF_WIN_SEL, g_MCCS_parameter.u8RGBGain[0], g_MCCS_parameter.u8RGBGain[1], g_MCCS_parameter.u8RGBGain[2]);
    }

    if (1 == g_MCCS_parameter.bRGBOffsetModified)
    {
        msAPI_AdjustBlacklevel(USER_PREF_WIN_SEL, g_MCCS_parameter.u8RGBOffset[0], g_MCCS_parameter.u8RGBOffset[1], g_MCCS_parameter.u8RGBOffset[2]);
    }

    if (1 == g_MCCS_parameter.bColorMatrixModified)
    {
        int idx = 0;
        short* psMatrix = &g_MCCS_parameter.ColorMatrix[0];
        short val = 0;
        // 2' complement  format transfer to short data format;
        for (idx = 0; idx < 9; idx++)
        {
            val = psMatrix[idx];
            //LUT_printData("val -before %d", val);
            if ((val & _BIT12) == _BIT12)
            {
                val &= 0xFFF;
                val = 0x1000 - val;
                val *= -1;
            }
            psMatrix[idx] = val;
            //LUT_printData("val -after %d", val);
        }
        msWritexvYccColorMatrix(USER_PREF_WIN_SEL, psMatrix);
        msEnablexvYccColorMatrix( USER_PREF_WIN_SEL, true);
    }

    if (1 == g_MCCS_parameter.bBrightnessModified)
    {
        msAPI_AdjustBrightness(g_MCCS_parameter.u8Brightness);
    }

    if (1 == g_MCCS_parameter.bContrastModified)
    {
        msAPI_AdjustContrast(USER_PREF_WIN_SEL , g_MCCS_parameter.u8Contrast);
    }

    if (1 == g_MCCS_parameter.bSaturationModified)
    {
        msAPI_AdjustSaturation(USER_PREF_WIN_SEL, g_MCCS_parameter.u8Saturation);
    }

    if (1 == g_MCCS_parameter.bHueModified)
    {
        msAPI_AdjustVideoHue(USER_PREF_WIN_SEL, GetProperHue(g_MCCS_parameter.u8Hue));
    }

}

#endif

