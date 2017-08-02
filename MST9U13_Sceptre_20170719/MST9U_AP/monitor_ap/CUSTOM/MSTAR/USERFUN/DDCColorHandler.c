#include "DDCColorHandler.h"
#include "msflash.h"
#include "Utl.h"
#include "sysinfo2_MST9U.h"

#define CommonVersion   "1.1.1"
WORD _checkSum;
WORD _status;
SendFileConfig _fileCfg = {MS_TOOL_ADR  |0x80000000, /*mem addr |0x80000000 PA2VA*/
                                            0x0000, /*tx header oft*/
                                            0x1000, /*rx header oft*/
                                            0x2000, /*header size*/
                                            0, 0, 0, 0, 0, 0};

BYTE CommonHandler(void);
BOOL IsCommonCommand (BYTE CommandCode);

const BYTE CommonCommand[] = {
    MS_EN_INTERNAL_PATTERN,
    MS_SET_INTERNAL_PATTERN,
    MS_EN_COLOR_ENGINE_PATTERN,
    MS_SET_COLOR_ENGINE_PATTERN,
    MS_GET_MODEL_NAME,
    MS_BRIGHTNESS,
    MS_ColorEngine_OnOff,
    MS_Read_ACK,
    MS_WR_BLOCK,
    MS_DDCWriteReg,
    MS_DDCReadReg,
    MS_GetToolFlagStatus,
    MS_GetCommonVersion,
    MS_RunISP,
    MS_GetMemAddr,
    MS_Read_Status
};

void ColorFuncNVRamSizeCheck(void)
{
    // To be noticed! This function is used to check NVRam size for color functions.
    // If the address of color data block is greater than NVRam size, it will trigger compile error.
    // Consequently, developer have to check whether whole of the EEPROM data structures(EepromMap.h) are necessary.
    // And also check if NVRam size is expected.

    #if ENABLE_DEMURA_FUNCTION
        BUILD_BUG_ON((ADDR_DEMURA_BLOCK + CNT_DEMURA_BLOCK) > NVRAM_SIZE);
    #endif

    #if ENABLE_DELTAE_FUNCTION
        BUILD_BUG_ON((ADDR_DELTAE_BLOCK + CNT_DELTAE_BLOCK) > NVRAM_SIZE);
    #endif

    #if (ENABLE_COLOR_CALIBRATION_FOR_THIRD_PARTY && ENABLE_SaveMCCSData)
        BUILD_BUG_ON((ADDR_THIRD_PARTY_BLOCK + CNT_THIRD_PARTY_BLOCK) > NVRAM_SIZE);
    #endif
}

BYTE ColorCalibrationHandler(BYTE CommandCode)
{
    if (CommandCode == MStar_COMMAND)
    {
        BYTE subCode = DDCBuffer[2];
        if (IsCommonCommand(subCode))
        {
            return CommonHandler();
        }
        else if (IsAutoColorCommand(subCode))
        {
            return AutoColorCalibrationHandler(&_checkSum);
        }
        else if (IsDemuraCommand(subCode))
        {
            return DemuraCalibrationHandler(&_checkSum,&_status);
        }

    }
    #if ENABLE_COLOR_CALIBRATION_FOR_THIRD_PARTY
    else if (CommandCode == MCCS_COLORAP_COMMAND_READ || CommandCode == MCCS_COLORAP_COMMAND_WRITE)
    {
        return MCCSCalibrationHandler(CommandCode);
    }
    #endif
    return 0;
}

DWORD Mem_GetTxFileSize(void)
{
    DWORD val = 0;
    memcpy(&val, &_fileCfg.pBuffer[_fileCfg.txHeaderOft + 0], 4);
    return val;
}

DWORD Mem_GetTxSentFileLen(void)
{
    DWORD val = 0;
    memcpy(&val, &_fileCfg.pBuffer[_fileCfg.txHeaderOft + 4], 4);
    return val;
}

BOOL Mem_GetTxTerminatedState(void)
{
    DWORD val = 0;
    memcpy(&val, &_fileCfg.pBuffer[_fileCfg.txHeaderOft + 8], 4);
    if (val == 0)
        return FALSE;
    else
        return TRUE;
}

void Mem_SetRxRecvFileLen(DWORD val)
{
    _fileCfg.fileRecvLen = val;
    memcpy(&_fileCfg.pBuffer[_fileCfg.rxHeaderOft + 0], &val, 4);
}

void Mem_SetRxCheckSum(DWORD val)
{
    _fileCfg.checkSum = val;
    memcpy(&_fileCfg.pBuffer[_fileCfg.rxHeaderOft + 4], &val, 4);
}

void Mem_SetRxTerminatedState(BOOL IsTerminated)
{
    DWORD val = 0;
    if (IsTerminated)
    {
        val =1;
    }
    memcpy(&_fileCfg.pBuffer[_fileCfg.rxHeaderOft + 8], &val , 4);
}

void PartialFlashUpdate( Bool bDoWP, DWORD dwAddr, BYTE *buffer, DWORD count, Bool bChipErase)
{
       msWriteByte(REG_000393, 0x5F);      /* Stop 51 */
       msWriteBit(REG_00038A, 0, BIT6);
       msWriteByte(REG_000AE4, 0x00);      /* Set SPI to normal mode */
       FlashBurstWrite(bDoWP, dwAddr, buffer, count, bChipErase);
}

BYTE CommonHandler(void)
{
    switch(DDCBuffer[2])
    {
        case MS_EN_INTERNAL_PATTERN :
        {
            if ( DDCBuffer[3] == 0x00 )
            {
                //Off
                msSetTestPattern(FALSE, 0, 0, 0);
            }
            else
            {
                //On
                msSetTestPattern(TRUE, 0, 0, 0);
            }
        }
        break;

        case MS_SET_INTERNAL_PATTERN :
        {
            msSetTestPattern(TRUE, DDCBuffer[3], DDCBuffer[4], DDCBuffer[5]);
        }
        break;

	 case MS_Read_Status:
        {
            DDCBuffer[2] = _status>>8;
            DDCBuffer[1] = _status & 0xFF;
            DDCBuffer[0] = 2;
            return 2;
        }
        break;

        case MS_Read_ACK :
        {
            DDCBuffer[2] = _checkSum >>8;
            DDCBuffer[1] = _checkSum & 0xFF;
            DDCBuffer[0] = 2;
            return 2;
        }
        break;

        case MS_GetToolFlagStatus :
        {
            DDCBuffer[1] = bRunToolFlag & 0xFF;
            DDCBuffer[0] = 1;
            return 1;
        }
        break;

        case MS_WR_BLOCK :
        {
            bRunToolFlag = DDCBuffer[3] ;
        }
        break;

        case MS_EN_COLOR_ENGINE_PATTERN :
        {
            if ( DDCBuffer[3] == 0x00 )
            {
                //Off
                msSetColorEngineTestPattern(USER_PREF_WIN_SEL, _DISABLE, 0x00, 0x00, 0x00);
            }
            else
            {
                //On
                msSetColorEngineTestPattern(USER_PREF_WIN_SEL, _ENABLE, 0x00, 0x00, 0x00);
            }
        }
        break;

        case MS_SET_COLOR_ENGINE_PATTERN :
        {
            msSetColorEngineTestPattern(USER_PREF_WIN_SEL, _ENABLE, (DDCBuffer[3]<<8|DDCBuffer[4]), (DDCBuffer[5]<<8|DDCBuffer[6]), (DDCBuffer[7]<<8|DDCBuffer[8]));
        }
        break;

        case MS_GET_MODEL_NAME :
        {
            int idx =0, model_len = 0;

            while(tool_model_panel_name[model_len])
                ++model_len;
            ++model_len; //for the \0

            DDCBuffer[0] =model_len;
            for( idx = 0 ; idx < model_len-1 ; ++idx)
                  DDCBuffer[idx+1] = tool_model_panel_name[idx];
            DDCBuffer[model_len] = 0x00;
            return model_len;

        }
        break;

        case MS_BRIGHTNESS:
        {
            BYTE command_write = DDCBuffer[3];
            BYTE value = DDCBuffer[4];
            if(command_write)
            {
    		USER_PREF_BRIGHTNESS = value;
    		msAPI_AdjustBrightness(USER_PREF_BRIGHTNESS);
            }
            else
            {
                BYTE chksum = USER_PREF_BRIGHTNESS;
                DDCBuffer[1] = chksum & 0xFF;
                DDCBuffer[0] = 1;
                return 1;
            }
        }
        break;

        case MS_DDCWriteReg:
        {
              DWORD addr = (DDCBuffer[3]<<24)+(DDCBuffer[4]<<16) + (DDCBuffer[5]<<8)+DDCBuffer[6] ;
              BYTE value = DDCBuffer[7];
              msDDCWriteReg(USER_PREF_WIN_SEL, addr,value);
        }
        break;

        case MS_DDCReadReg:
        {
              DWORD addr = (DDCBuffer[3]<<24)+(DDCBuffer[4]<<16) + (DDCBuffer[5]<<8)+DDCBuffer[6] ;
              BYTE value = msDDCReadReg(USER_PREF_WIN_SEL,addr);
              DDCBuffer[1] = value;
              DDCBuffer[0] = 1;
              return 1;
        }
        break;

        case MS_GetCommonVersion:
        {
            int idx =0, strVerlen = 0;

            strVerlen = sizeof(CommonVersion);

            DDCBuffer[0] =strVerlen;
            for( idx = 0 ; idx < strVerlen-1 ; ++idx)
                  DDCBuffer[idx+1] = CommonVersion[idx];
            DDCBuffer[strVerlen] = 0x00;
            return strVerlen;
        }
        break;
        case MS_GetMemAddr:
        {
            //little endian
            DDCBuffer[4] = (BYTE)(_fileCfg.memAddr & 0xFF);
            DDCBuffer[3] = (BYTE)(_fileCfg.memAddr >> 8);
            DDCBuffer[2] = (BYTE)(_fileCfg.memAddr >> 16);
            DDCBuffer[1] = (BYTE)((_fileCfg.memAddr >> 24) & 0x7F);
            DDCBuffer[0] = 4;
            return 4;
        };
        case MS_RunISP:
        {
            DWORD wrLen = 0, idx = 0, flashWrAddr = 0, fileSize = 0;
            _fileCfg.pBuffer = (BYTE*)_fileCfg.memAddr;
             bRunToolFlag = TRUE;
            Mem_SetRxRecvFileLen(0);
            Mem_SetRxCheckSum(0);
            Mem_SetRxTerminatedState(FALSE);
            _fileCfg.fileSize = Mem_GetTxFileSize();

            fileSize =  DDCBuffer[6] + (((DWORD)DDCBuffer[5]) << 8)
                                                                    + (((DWORD)DDCBuffer[4]) << 16)
                                                                    + (((DWORD)DDCBuffer[3]) << 24);

            flashWrAddr =  DDCBuffer[10] + (((DWORD)DDCBuffer[9]) << 8)
                                                                              + (((DWORD)DDCBuffer[8]) << 16)
                                                                              + (((DWORD)DDCBuffer[7]) << 24);

            if (fileSize != _fileCfg.fileSize)
            {
                Mem_SetRxTerminatedState(TRUE);
                bRunToolFlag = FALSE;
                return 0;
            }

            while((_fileCfg.fileSize > _fileCfg.fileRecvLen) && (_fileCfg.bTerminated == FALSE))
            {
                _fileCfg.fileSentLen = Mem_GetTxSentFileLen();
                _fileCfg.bTerminated = Mem_GetTxTerminatedState();

                if (((_fileCfg.fileSentLen - _fileCfg.fileRecvLen) >= 0x1000) || (_fileCfg.fileSentLen == _fileCfg.fileSize))
                {
                    if (_fileCfg.fileSentLen == _fileCfg.fileSize)
                    {
                        wrLen = _fileCfg.fileSentLen - _fileCfg.fileRecvLen;
                    }else{
                        wrLen = ((_fileCfg.fileSentLen - _fileCfg.fileRecvLen) / 0x1000) * 0x1000;
                    }

                    PartialFlashUpdate(TRUE, /*Write Potect*/
                                               flashWrAddr + _fileCfg.fileRecvLen/*offset*/,
                                               (BYTE*)(&_fileCfg.pBuffer[_fileCfg.fileRecvLen + _fileCfg.headerSize])/*write data*/,
                                              wrLen /*size*/,
                                               FALSE /*chip erase*/);

                    _fileCfg.fileRecvLen += wrLen;
                    Mem_SetRxRecvFileLen(_fileCfg.fileRecvLen);
                }

            }

            //Tool can read the checksum to judge the FILE send result is OK or NG.
            for(idx = 0; idx < _fileCfg.fileSize; idx++)
            {
                 _fileCfg.checkSum ^= _fileCfg.pBuffer[idx + _fileCfg.headerSize];
            }
            Mem_SetRxCheckSum(_fileCfg.checkSum);
            bRunToolFlag = FALSE;
        }
    }
    return 0;
}

BOOL IsCommonCommand (BYTE CommandCode)
{
    BYTE idx = 0;
    for (idx = 0; idx < sizeof(CommonCommand)/sizeof(BYTE); idx++)
    {
        if (CommonCommand[idx] == CommandCode)
            return TRUE;
    }
    return FALSE;
}

void msDDCWriteReg(BYTE u8DispWin,DWORD addr,BYTE value)
{
    BYTE u8ScalerIdx = 0;
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8DispWin));
    WORD u16ScalerMask = g_DisplayWindow[u8DispWin].u16DwScMaskOut;

    if (addr & SC00_00)
    {
        while(u16ScalerMask)
        {
            if(u16ScalerMask & BIT0)
            {
                 u32BaseAddr = g_ScalerInfo[u8ScalerIdx].u32SiRegBase;
                 msWriteByte(u32BaseAddr+addr, value);
            }
            u8ScalerIdx++;
            u16ScalerMask >>= 1;
        }
    }
    else
        msWriteByte(addr, value);


}
BYTE msDDCReadReg(BYTE u8DispWin,DWORD addr)
{
    DWORD u32BaseAddr = SCRegBase(MapWin2ScIndex(u8DispWin));

     if (addr & SC00_00)
       return msReadByte(addr+u32BaseAddr);
     else
       return msReadByte(addr);
}
