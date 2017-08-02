//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#define MW_USBDOWNLOAD_C

/******************************************************************************/
/*                              Header Files                                  */
/******************************************************************************/
// C Library
#include <stdio.h>
#include <string.h>

// Global Layer
#include "datatype.h"
#include "MsTypes.h"
#include "imginfo.h"
#include "debug.h"
#include "Board.h"
#include "ms_reg.h"
#include "Ms_rwreg.h"

// Driver Layer
#include "drvUSB.h"
#include "drvBDMA.h"

// API Layer
#include "msAPI_FCtrl.h"
#include "msAPI_MSDCtrl.h"
#include "msAPI_Memory.h"

// MW Layer
#include "mw_usbdownload.h"

// APP Layer
#include "MApp_SwUpdate.h"
#include "msAPI_Timer.h"

#if(ENABLE_6M30_3D_PROCESS)
#include "drvUrsa6M30.h"
#include "MApp_Isp.h"
#endif

#include "msflash.h"
//leo.chou need to implement the following requirement..
#define IsStorageInUse()      (FALSE)//(msAPI_InputSrcType_InUsedType(EN_SYS_INPUT_SOURCE_TYPE_STORAGE))


#if(ENABLE_USB_DOWNLOAD_BIN)
#include "drvSERFLASH.h"
#include "BinInfo.h"
#include "SysInit.h"
#include "msAPI_BDMA.h"
#include "imginfo.h"
#endif


/******************************************************************************/
/*                                 Enum                                       */
/******************************************************************************/

/******************************************************************************/
/*                                 Macro                                      */
/******************************************************************************/

// Debug message for USB Download
#define USB_DL_DBG(x)           x
#define USB_DL_ASSERT(x)        x

// Max USB detection duration
#define MAX_USB_DETECTION_DURATION  2000    // ms
#define XD2PHY(xadr)        ((U32)(xadr))

/******************************************************************************/
/*                                 Structure                                  */
/******************************************************************************/

/******************************************************************************/
/*                             Public Variable                                */
/******************************************************************************/
pZUIDrawCB pZUIDrawPercentageCB = 0;

/******************************************************************************/
/*                             Private Variable                               */
/******************************************************************************/
static U8 _u8USBEnvironmentID = 0xFF;
static U8 _u8USBStatus = 0;
static U8 _u8USBPortNo = 0;
static U8 _u8UpgradeMode = 0;
static U8 _u8MemType = 0;
static U8 _u8FlashID = 0;
static U8 _pu8FileName[MAX_FILENAME_LEN] ={0};
static U32 _u32DRAMAddr = 0;
static U32 _u32DRAMLen = 0;
static U32 _u32FlashSize = 0;
static U32 _u32DownloadSize = 0;
static U32 _u32FileSize = 0;

static BOOLEAN  _bUsbInitialized = FALSE;
static BOOLEAN  _bAPBinFlag = FALSE;
static BOOLEAN  _bFlashID1 = FALSE;
static BOOLEAN  _bFlashID2 = FALSE;
static FileEntry _fileEntry2;
static FileEntry _fileEntry;

/******************************************************************************/
/*                           Public Function                                  */
/******************************************************************************/
void MW_UsbDownload_Init(U8 u8USBPortNo, U8 u8FlashID, U32 u32DRAMAddr, U32 u32DRAMLen, U32 u32FlashSize, U8 u8UpgradeMode, U8 u8MemType, U8 * pu8FileName, pZUIDrawCB ZUIDrawPercentage, BOOLEAN bAPBinFlag);
BOOLEAN MW_UsbDownload_Search(void);
BOOLEAN MW_UsbDownload_Start(BOOL bFullDL);

/******************************************************************************/
/*                             Private Function                                */
/******************************************************************************/
static BOOLEAN _MW_UsbDownload_StartDevice(S8 deviceIndex);
static BOOLEAN _MW_UsbDownload_SearchFileInRoot(U8* pu8FileName, FileEntry* pFileEntry);
static BOOLEAN _MW_UsbDownload_Copy(U32 u32DRAMAddr, U32 u32DRAMLen, U32 u32FlashSize);
static BOOLEAN _MW_UsbDownload_InitFileSystem(void);
static void _MW_UsbDownload_Exit(void);
static BOOLEAN _MW_UsbDownload_CheckHeader(void);

/******************************************************************************/
/*                          External Function                                 */
/******************************************************************************/
extern void ASCIItoUnicode2(S8 *ps8FileName, U8 len);
extern U32 msAPI_Timer_GetTime0(void);
extern U32 msAPI_Timer_DiffTimeFromNow(U32 u32TaskTimer);
extern BOOLEAN MApp_MassStorage_DeviceDisconnect(void);
extern void MApp_UsbClearWatchDog(void);
#if (OBA2)
extern BOOLEAN MApp_Obama_SwUpdate(U32 u32Addr, U32 u32Size);
extern U32 MApp_UsbPA2VA(U32 u32x);
#endif

/******************************************************************************/
/*                             Code Implementation                            */
/******************************************************************************/
static BOOLEAN _MW_UsbDownload_StartDevice(S8 deviceIndex)
{
    EN_MSD_STATUS status=msAPI_MSDCtrl_GetDeviceStatus(deviceIndex);

    USB_DL_DBG(printf("deviceStatus=%d for deviceIndex=%d\n",(U8)status,(U8)deviceIndex));

    if(status==MSD_STATUS_INSERTED)
    {
        return msAPI_FCtrl_ActiveDevice(deviceIndex);
    }
    else if(status==MSD_STATUS_IDLE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static BOOLEAN _MW_UsbDownload_SearchFileInRoot(U8* pu8FileName, FileEntry* pFileEntry)
{
    U8  len;
    U8 *pu8Name;

    BOOLEAN result=FALSE;
    U16 *pFileNameToCompare=NULL;
    #if ENABLE_USB_DOWNLOAD_USE_VERSION_BY_FILENAME
    U16 *pPreFileNameToCompare=NULL;
    #endif
    BOOL bisDir = FALSE;
    U8 u8dirIndex=0;
    U8 u8FileExtIndex=0;

    USB_DL_DBG(printf("_MW_UsbDownload_SearchFileInRoot(pu8FileName=%s)\n", pu8FileName));

    len = strlen((char *)pu8FileName);

    if(len==0)
    {
        USB_DL_ASSERT(printf("Error> File name length is 0\n"));
        return FALSE;
    }

    if(!msAPI_FCtrl_DirectoryChangeToRoot(_u8USBEnvironmentID))
    {
        USB_DL_ASSERT(printf("Error> msAPI_FCtrl_DirectoryChangeToRoot failed\n"));
        return FALSE;
    }

    pu8Name=(U8 *)msAPI_Memory_Allocate(128*2, BUF_ID_USB_DOWNLOAD);
    if(pu8Name==NULL)
    {
        __ASSERT(0);
        USB_DL_ASSERT(printf("Error> 1. msAPI_Memory_Allocate failed\n"));
        return FALSE;
    }

    memset(pu8Name, 0, sizeof(pu8Name));

    pFileNameToCompare=(U16 *)msAPI_Memory_Allocate(128*2, BUF_ID_USB_DOWNLOAD);
    if(pFileNameToCompare==NULL)
    {
        __ASSERT(0);
        if (pu8Name != NULL)
        {
            msAPI_Memory_Free(pu8Name, BUF_ID_USB_DOWNLOAD);
            pu8Name = NULL;
        }
        USB_DL_ASSERT(printf("Error> 2. msAPI_Memory_Allocate failed\n"));
        return FALSE;
    }
    
#if ENABLE_USB_DOWNLOAD_USE_VERSION_BY_FILENAME
    FileEntry *tempFileEntry = NULL;
	tempFileEntry = malloc(sizeof(FileEntry));
    pPreFileNameToCompare=(U16 *)msAPI_Memory_Allocate(128*2, BUF_ID_USB_DOWNLOAD);
    if(pPreFileNameToCompare==NULL)
    {
        __ASSERT(0);
        if (pu8Name != NULL)
        {
            msAPI_Memory_Free(pu8Name, BUF_ID_USB_DOWNLOAD);
            pu8Name = NULL;
        }
        USB_DL_ASSERT(printf("Error> 2.1 msAPI_Memory_Allocate failed\n"));
        return FALSE;
    }
	memset(pPreFileNameToCompare,0,128*2);
#endif
searchDir:
    bisDir = FALSE;
    for(len=0;u8dirIndex<strlen((char *)pu8FileName);len++,u8dirIndex++)  // stretch file /directory name
    {
        if(pu8FileName[u8dirIndex]==0x5C && !bisDir) // if '\' then its a directory
        {           
            bisDir = TRUE;
            u8FileExtIndex = 0xFF;
            u8dirIndex++;
            break;
        }
        else if(pu8FileName[u8dirIndex]!=0)
        {
          if(pu8FileName[u8dirIndex] == 0x2E) // if "." indicate the filename extension
            u8FileExtIndex = len;
          pu8Name[len] = pu8FileName[u8dirIndex];   
        }
    }
    //ASCIItoUnicode2((S8 *)pu8Name, strlen((char *)pu8Name));
nextentry:
    while(msAPI_FCtrl_EntryNext(_u8USBEnvironmentID,pFileEntry)==ENTRY_NEXT_RESULT_SUCCESS)
    {
        U8 lenToCompare;

        MApp_UsbClearWatchDog();
#if (OBA2)
        BOOL bShortNameOnly;
        lenToCompare = msAPI_FCtrl_EntryGetLongNameByFileEntry(_u8USBEnvironmentID,
                    pFileEntry, XD2PHY(pFileNameToCompare), 127, &bShortNameOnly);
#else
        lenToCompare = msAPI_FCtrl_EntryGetCurrentName(_u8USBEnvironmentID, XD2PHY(pFileNameToCompare), 127);
#endif
        if(!bisDir || lenToCompare==len)
        {
            U8 i; 
            U16 c1, c2; 
        #if ENABLE_USB_DOWNLOAD_USE_VERSION_BY_FILENAME
            U16 c3;
            U8 j;
            i = len;
            j = lenToCompare;
            while(i>0 && j>0 && pFileNameToCompare[j-1] != 0x2E &&  !bisDir) // compare the Filename extension match or not
            {
                c1 = pu8Name[--i];
                c2 = pFileNameToCompare[--j];
                if ((c1 >= 'a') && (c1 <= 'z'))
                {
                    c1 -= 'a' - 'A';
                }
                if ((c2 >= 'a') && (c2 <= 'z'))
                {
                    c2 -= 'a' - 'A';
                }
                if (c1 != c2 )
                {
                    goto nextentry;
                }
            }
        #endif
            for(i=0; i<lenToCompare; i++)
            {
                c1 = pu8Name[i];
                c2 = pFileNameToCompare[i];

                if ((c1 >= 'a') && (c1 <= 'z'))
                {
                    c1 -= 'a' - 'A';
                }

                if ((c2 >= 'a') && (c2 <= 'z'))
                {
                    c2 -= 'a' - 'A';
                }

                if (c1 != c2 )
                {
                #if ENABLE_USB_DOWNLOAD_USE_VERSION_BY_FILENAME  // when the filename match the format, we compare the rest number of the filename and find the maximum one
                    c3 = pPreFileNameToCompare[i];
					if(i>=u8FileExtIndex && c2 > c3)
                    {
                        memset(pPreFileNameToCompare,0,128*2);
                        memcpy(pPreFileNameToCompare,pFileNameToCompare,128*2);
                        memcpy(tempFileEntry,pFileEntry,sizeof(FileEntry));
                        result = TRUE;
                        goto nextentry;
                    }
                    else if(i>=u8FileExtIndex && c2 == c3)
                    {
                        continue;
                    }
                    else
                #endif
                    goto nextentry;
                }

            }
            if(bisDir)
            {
                if(!msAPI_FCtrl_DirectoryChange(_u8USBEnvironmentID,pFileEntry))
                {
                    printf("can not access direcory %s\n",pu8Name);
                    result = FALSE;
                    break;
                }
                goto searchDir;
            }   
            USB_DL_DBG(printf("*Found %s\n", pu8FileName));

            result=TRUE;
            break;
        }
    }
    #if ENABLE_USB_DOWNLOAD_USE_VERSION_BY_FILENAME
    if(result) // return the max number of filename we found
    {
        memcpy(pFileEntry,tempFileEntry,sizeof(FileEntry));
    }
	free(tempFileEntry);
	tempFileEntry = NULL;
    msAPI_Memory_Free(pPreFileNameToCompare, BUF_ID_USB_DOWNLOAD);
    pPreFileNameToCompare = NULL;
    #endif
    msAPI_Memory_Free(pFileNameToCompare, BUF_ID_USB_DOWNLOAD);
    pFileNameToCompare = NULL;

    msAPI_Memory_Free(pu8Name, BUF_ID_USB_DOWNLOAD);
    pu8Name = NULL;

    return result;
}

static BOOLEAN _MW_UsbDownload_Copy(U32 u32DRAMAddr, U32 u32DRAMLen, U32 u32FlashSize)
{
    U8      u8HandleNo;
    U32     u32FileSize = 0;
    U32     u32LengthLeft = 0;

    u8HandleNo = msAPI_FCtrl_FileOpen(&_fileEntry, OPEN_MODE_FOR_READ);
    if(u8HandleNo == FCTRL_INVALID_FILE_HANDLE)
    {
        USB_DL_ASSERT(printf("Error> Image file open failed\n"));
        return FALSE;
    }

    u32FileSize = msAPI_FCtrl_FileLength(u8HandleNo);
    _u32FileSize = u32FileSize;
    printf("File size: 0x%08lx\n", u32FileSize);
    printf("Flash size: 0x%08lx\n", u32FlashSize);
    printf("Download Buffer Addr: 0x%08lx\n", u32DRAMAddr);
    printf("Download Buffer Size: 0x%08lx\n", u32DRAMLen);

    if ((u32FileSize == 0) || (u32FileSize > u32FlashSize) || (u32FileSize > u32DRAMLen))
    {
        USB_DL_ASSERT(printf("Error> File size error\n"));
        msAPI_FCtrl_FileClose(u8HandleNo);
        return FALSE;
    }

    MApp_UsbClearWatchDog();

    printf("Copying image from USB to DRAM\n");

    u32LengthLeft = msAPI_FCtrl_FileRead(u8HandleNo, u32DRAMAddr, u32FileSize);
    msAPI_FCtrl_FileClose(u8HandleNo);

  #if (ENABLE_6M30_3D_PROCESS)
    if (MDrv_Ursa_USB_Update_GetChipType() == SWUpdateForMFC)
    {
          MApp_MFC_ISP_Init(u32DRAMAddr, u32FileSize);

          if(u32LengthLeft != 0)
          {
               USB_DL_ASSERT(printf("Error> Read MFC file failed\n"));
               return FALSE;
          }
       else
          {
               USB_DL_ASSERT(printf("OK> Read MFC file completed\n"));
               return TRUE;
          }
    }
 #endif

    if(u32LengthLeft == 0)
    {
        _u32DownloadSize = u32FileSize;

        USB_DL_DBG( printf(" _u32DownloadSize=%lu\n", _u32DownloadSize); );

    #if (!OBA2)
        if(0x0 != MDrv_BDMA_CRC32(u32DRAMAddr, _u32DownloadSize, BDMA_CRC32_POLY, BDMA_CRC_SEED_0, E_BDMA_SRCDEV_MIU0, FALSE))
        {
            USB_DL_ASSERT(printf("Error> CRC32 Integrity Check failed !!\n"));
            return FALSE;
        }
        else
        {
            printf("CRC32 Integrity Check pass !!\n");
        }
    #else
        if (!MApp_Obama_SwUpdate(MApp_UsbPA2VA(u32DRAMAddr), _u32DownloadSize))
        {
           USB_DL_ASSERT(printf("Error> Obama USB upgrade failed !!\n"));
           return FALSE;
        }
    #endif
    }
    else
    {
        USB_DL_ASSERT(printf("Error> Read file failed\n"));
        return FALSE;
    }

    return TRUE;
}

/////////////////////////////////////////////////////
static BOOLEAN _MW_UsbDownload_InitFileSystem(void)
{
    USB_DL_DBG(printf("_MW_UsbDownload_InitFileSystem()\n"));

    _bUsbInitialized = FALSE;
    _u8USBEnvironmentID = 0xFF;
    int i;

    msAPI_FS_Init();
    msAPI_MSDCtrl_CheckDevices();

    if (_u8USBPortNo == 0x1)
    {
        //EN_MSD_STATUS DeviceStatus = msAPI_MSDCtrl_GetDeviceStatus(MSD_USB_INDEX_START);
        for(i = MSD_USB_INDEX_START; i < MSD_USB_INDEX_START + MSD_USB_NUM; i++)
        {
            if(msAPI_MSDCtrl_GetDeviceStatus(i)==MSD_STATUS_INSERTED && _MW_UsbDownload_StartDevice(i))
            {
                _u8USBEnvironmentID=msAPI_FSEnv_Register(0x00 /*'A'*/);
                _bUsbInitialized=msAPI_FCtrl_DirectoryChangeToRoot(_u8USBEnvironmentID);
             break;
            }
        }
        if(!_bUsbInitialized)
        {
            USB_DL_ASSERT(printf("Error> USB PORT 1 DEVICE DETECT ERROR!!\n"));
        }
    }
    else if (_u8USBPortNo == 0x2)
    {
        for(i = MSD_USB_2_INDEX_START; i < MSD_USB_2_INDEX_START + MSD_USB_2_NUM; i++)
        {
            if(msAPI_MSDCtrl_GetDeviceStatus(i)==MSD_STATUS_INSERTED && _MW_UsbDownload_StartDevice(i))
        {
            _u8USBEnvironmentID=msAPI_FSEnv_Register(0x00 /*'A'*/);
            _bUsbInitialized=msAPI_FCtrl_DirectoryChangeToRoot(_u8USBEnvironmentID);
             break;
        }
        }
        if(!_bUsbInitialized)
        {
            USB_DL_ASSERT(printf("Error> USB PORT 2 DEVICE DETECT ERROR!!\n"));
        }
    }
    else
    {
        USB_DL_ASSERT(printf("Error> USB PORT NUMBER ERROR!!\n"));
    }

    if (_bUsbInitialized)
    {
        return TRUE;
    }
    else
    {
        USB_DL_ASSERT(printf("Error> File system init error\n"));
        return FALSE;
    }
}

static void _MW_UsbDownload_Exit(void)
{
    USB_DL_DBG(printf("_MW_UsbDownload_Exit\n"));
    if (_bUsbInitialized)
    {
        //_bUsbInitialized = FALSE;
        //msAPI_FCtrl_FileHandleResetAll();
        msAPI_FSEnv_UnRegister(_u8USBEnvironmentID);

        //MApp_MassStorage_DeviceDisconnect();
        //msAPI_FCtrl_Init();
    }
}

static BOOLEAN _MW_UsbDownload_CheckHeader(void)
{
    U8    u8HandleNo;
    U32  u32LengthLeft;
    U32  u32FileSize;

    u8HandleNo = msAPI_FCtrl_FileOpen(&_fileEntry2, OPEN_MODE_FOR_READ);
    if(u8HandleNo == FCTRL_INVALID_FILE_HANDLE)
    {
        USB_DL_ASSERT(printf("Error> Open the 2nd flash BIN file failed\n"));
        return FALSE;
    }

    u32FileSize = msAPI_FCtrl_FileLength(u8HandleNo);
    USB_DL_DBG( printf("File size: %d ; Download Buffer Size: %d\n", u32FileSize, _u32DRAMLen) );

    u32LengthLeft = msAPI_FCtrl_FileRead(u8HandleNo, _u32DRAMAddr, u32FileSize);
    if(u32LengthLeft == 0)
    {
        _u32DownloadSize = u32FileSize;
    }
    else
    {
        USB_DL_ASSERT(printf("Error> Read the 2nd flash BIN file failed\n"));
    }

    msAPI_FCtrl_FileClose(u8HandleNo);

    // Check BIN file header: the first 5 bytes
    if((*((U8 *)_u32DRAMAddr) == 0x09) &&
      (*((U8 *)_u32DRAMAddr+1) == 0x58) &&
      (*((U8 *)_u32DRAMAddr+2) == 0x33) &&
      (*((U8 *)_u32DRAMAddr+3) == 0x69)&&
      (*((U8 *)_u32DRAMAddr+4) == 0x00))
    {
        //USB_DL_DBG(printf("Find the 2nd flash BIN file header\n"));
        return TRUE;
    }
    else
    {
        USB_DL_ASSERT(printf("Error> Read the 2nd flash BIN file header failed\n"));
        return FALSE;
    }
}
//****************************************************************************
/// MW_UsbDownload_GetVersion
/// @param
///     version: xxxxxx-xxxxx
///     verval: XXXX
///-@see
/// @return True
//****************************************************************************
BOOL MW_UsbDownload_GetVersion(U8 *u8modelName, U32 *u32Version)
{
    BYTE u8HandleNo=0;
    DWORD u8ModelNameAdd=0,u8FWversionAdd=0;
    WORD u32FileSize=0;
    MS_IMG_INFO ImgInfo;
    DWORD dwAPImgInfoAdd=0, dwAPStartAdd=0;

    u8ModelNameAdd=(DWORD)&ImgInfo.u8ModelName-(DWORD)&ImgInfo.u32Magic;
    u8FWversionAdd=(DWORD)&ImgInfo.u16SW_Version-(DWORD)&ImgInfo.u32Magic;

    if (_MW_UsbDownload_Copy(_u32DRAMAddr, _u32DRAMLen, _u32FlashSize))
    {
        USB_DL_DBG(printf("_MW_UsbDownload_Copy done, _u32DRAMAddr=0x%x, _u32DRAMLen=0x%x, _u32FlashSize=0x%x\n", _u32DRAMAddr, _u32DRAMLen, _u32FlashSize));

        u8HandleNo = msAPI_FCtrl_FileOpen(&_fileEntry,OPEN_MODE_FOR_READ);
        if (u8HandleNo == FCTRL_INVALID_FILE_HANDLE)
        {
            USB_DL_ASSERT(printf("Error> Open the 2nd flash BIN file failed \n"));
            return FALSE;
        }
        u32FileSize = msAPI_FCtrl_FileLength(u8HandleNo);
        USB_DL_DBG(printf("-------------->>File size: %d ; Download Buffer Size %d \n",u32FileSize, _u32DRAMLen));
    }

    dwAPStartAdd=(_u32DRAMAddr + Sboot_size);  // read AP code start address from chuck haeder in DRAM
    USB_DL_DBG(printf("AP code start address = 0x%x\n", dwAPStartAdd));

    memcpy(&dwAPImgInfoAdd, (const void *)dwAPStartAdd, 4);
    dwAPImgInfoAdd += 0x1100;
    USB_DL_DBG(printf("AP code image information address = 0x%x\n", dwAPImgInfoAdd));

    u8ModelNameAdd=(_u32DRAMAddr + dwAPImgInfoAdd + u8ModelNameAdd);
    u8FWversionAdd=(_u32DRAMAddr + dwAPImgInfoAdd + u8FWversionAdd);

    memcpy(u8modelName, (const void *)u8ModelNameAdd, sizeof (ImgInfo.u8ModelName));
    memcpy(u32Version, (const void *)u8FWversionAdd, sizeof (ImgInfo.u16SW_Version));
    return TRUE;
}


//****************************************************************************
/// USB Download Init
/// @param ZUIDrawPercentage IN: UI draw call back function pointer
/// -@see pZUIDrawCB
/// @return void
//****************************************************************************
void MW_UsbDownload_Init(U8 u8USBPortNo, U8 u8FlashID, U32 u32DRAMAddr, U32 u32DRAMLen, U32 u32FlashSize, U8 u8UpgradeMode, U8 u8MemType, U8 * pu8FileName, pZUIDrawCB ZUIDrawPercentage, BOOLEAN bAPBinFlag)
{
    USB_DL_DBG(printf("MW_UsbDownload_Init()\n"));
    USB_DL_DBG(printf(" u8USBPortNo=%d\n", u8USBPortNo); );
    USB_DL_DBG(printf(" u8FlashID=%d\n", u8FlashID); );
    USB_DL_DBG(printf(" u32DRAMAddr=0x%lx\n", u32DRAMAddr); );
    USB_DL_DBG(printf(" u32DRAMLen=0x%lx\n", u32DRAMLen); );
    USB_DL_DBG(printf(" u32FlashSize=0x%lx\n", u32FlashSize); );
    USB_DL_DBG(printf(" u8UpgradeMode=%d\n", u8UpgradeMode); );
    USB_DL_DBG(printf(" u8MemType=%d\n", u8MemType); );
    USB_DL_DBG(printf(" file name=%s\n", pu8FileName); );
    USB_DL_DBG(printf(" bAPBinFlag=%d\n", bAPBinFlag); );


    _u8USBStatus = 0;
    _u8USBPortNo = u8USBPortNo;
    _u8FlashID = u8FlashID;
    _u8MemType = u8MemType;
    _u8UpgradeMode = u8UpgradeMode;
    memcpy(_pu8FileName, pu8FileName, sizeof(_pu8FileName));
    _u32DRAMAddr = u32DRAMAddr;
    _u32DRAMLen = u32DRAMLen;
    _u32FlashSize = u32FlashSize;

    pZUIDrawPercentageCB = ZUIDrawPercentage;

    _bUsbInitialized = FALSE;

    if(bAPBinFlag == TRUE)
    {
        _bAPBinFlag = TRUE;
        _bFlashID1 = FALSE;
        _bFlashID2 = FALSE;
    }
}

//****************************************************************************
/// USB Download Search
/// @param void
/// @return BOOLEAN
/// - TURE : Search donwload file successfully.
/// - FALSE : Search download file failed.
//****************************************************************************
BOOLEAN MW_UsbDownload_Search(void)
{
    U8      u8HandleNo;
    U32     u32TimeStart = msAPI_Timer_GetTime0();

    USB_DL_DBG(printf("MW_UsbDownload_Search\n"));
    USB_DL_DBG(printf(" _u8USBPortNo=%u\n", _u8USBPortNo));

    MApp_UsbClearWatchDog();

    if(_u8USBPortNo == 0x1)
    {
        while (!MDrv_UsbDeviceConnect())
        {
            U32 u32Duration = msAPI_Timer_DiffTimeFromNow(u32TimeStart);
            MApp_UsbClearWatchDog();
            if (u32Duration > MAX_USB_DETECTION_DURATION)
            {
                USB_DL_ASSERT(printf("Error> USB Download Search: port 1 connection timeout\n"));
                return FALSE;
            }
        }
    }
#if (ENABLE_USB_2)
    else if(_u8USBPortNo == 0x2)
    {
        while (!MDrv_UsbDeviceConnect_Port2())
        {
            U32 u32Duration = msAPI_Timer_DiffTimeFromNow(u32TimeStart);
            MApp_UsbClearWatchDog();
            if (u32Duration > MAX_USB_DETECTION_DURATION)
            {
                USB_DL_ASSERT(printf("Error> USB Download Search: port 2 connection timeout\n"));
                return FALSE;
            }
        }
    }
#endif
    else
    {
        USB_DL_ASSERT(printf("Error> USB Download Search: port number error\n"));
        return FALSE;
    }

    // Init USB and file system, ...
    MApp_UsbClearWatchDog();

    if (!_MW_UsbDownload_InitFileSystem())
    {
        _MW_UsbDownload_Exit();
        return FALSE;
    }

    // Search files
    MApp_UsbClearWatchDog();

    if(_bAPBinFlag == TRUE)
    {
        if((_MW_UsbDownload_SearchFileInRoot((U8 *)(_pu8FileName), &_fileEntry2) == TRUE)&&(_MW_UsbDownload_CheckHeader() == TRUE))
        {
            _bFlashID2 = TRUE;
        }
        else
        {
            USB_DL_ASSERT(printf("Error> Search Merge2.bin failed\n"));
        }

        if(! _MW_UsbDownload_SearchFileInRoot((U8 *)(_pu8FileName), &_fileEntry))
        {
            USB_DL_ASSERT(printf("Error> Search Merge.bin failed\n"));
            if(! _MW_UsbDownload_SearchFileInRoot((U8 *)(_pu8FileName), &_fileEntry))
            {
                USB_DL_ASSERT(printf("Error> Search AP.bin failed\n"));
                if(_bFlashID2 == TRUE)
                {
                    return TRUE;// Only update BIN file
                }
                else
                {
                    return FALSE;
                }
            }
        }
        else
        {
            _bFlashID1 = TRUE;
        }
    }
    else
    {
        if(! _MW_UsbDownload_SearchFileInRoot((U8 *)(_pu8FileName), &_fileEntry))
        {
            if(! _MW_UsbDownload_SearchFileInRoot((U8 *)(_pu8FileName), &_fileEntry))
            {
                USB_DL_ASSERT(printf("Error> Search file failed\n"));
                if(!IsStorageInUse())
                {
                    _MW_UsbDownload_Exit();
                }
                return FALSE;
            }
        }
    }
	#if ENABLE_USB_DOWNLOAD_USE_AP_SW_VERSION
	U8 modelname[20]= "";
	U32 version=0;
	if(!MW_UsbDownload_GetVersion(modelname,&version))
	{
		USB_DL_ASSERT(printf("<Error> Gete Image Version failed!!\n"));
	}
	else
	{
		if(version <= AP_SW_VERSION)
		{
			USB_DL_DBG(printf("Already the latest version, USB: 0x%X, Current: 0x%X\n",version,AP_SW_VERSION));
			return FALSE;
		}
	}
	#endif

    u8HandleNo = msAPI_FCtrl_FileOpen(&_fileEntry, OPEN_MODE_FOR_READ);
    if(u8HandleNo == FCTRL_INVALID_FILE_HANDLE)
    {
        USB_DL_ASSERT(printf("Error> Open file failed\n"));
        _MW_UsbDownload_Exit();
        return FALSE;
    }

    msAPI_FCtrl_FileClose(u8HandleNo);
    _u8USBStatus=TRUE;

    USB_DL_DBG(printf("USB Download Search: OK\n"));

#if (ENABLE_6M30_3D_PROCESS)
    if(MDrv_Ursa_USB_Update_GetChipType() == SWUpdateForMFC)
    {
        USB_DL_DBG( printf("MFC image found\n") );
    }
    else
    {
         USB_DL_DBG( printf("Merged image found\n") );
    }
#endif

    return TRUE;
}


#if (ENABLE_6M30_3D_PROCESS)
BOOLEAN MApp_Ursa_SwUpdate_Start(void)
{
    U8 u8Percent;

    if (MDrv_Ursa_USB_Update_GetChipType() == SWUpdateForHK)
    {
      return FALSE;
    }

    do
    {
        u8Percent = MApp_MFC_ISP_Program();
    }while ((u8Percent < 0xF0) && (u8Percent != 0));

    if (u8Percent == 0)
    {
        printf("MFC.bin updating Success.\n");
        //sprintf(&(au8String[0]), "%s","Updated sucess!!!!");
        //USB_DL_ASSERT(printf("Updated sucess!!!!\n"));
        msAPI_Timer_Delayms(5000);
        return TRUE;
    }
    else
    {
        printf("MFC.bin updating Fail.\n");
        //sprintf(&(au8String[0]), "%s","Updated error!!!!");
        msAPI_Timer_Delayms(5000);
        return FALSE;
    }
}
#endif

//****************************************************************************
/// USB Download Start
/// @param void
/// @return BOOLEAN
/// - TRUE : USB download succeedfully.
/// - FALSE : USB download failed.
//****************************************************************************
BOOLEAN MW_UsbDownload_Start(BOOL bFullDL)
{
    U32 u32DownloadOffset=0;
    U32 u32DualImgOffset=0;

    USB_DL_DBG(printf("MW_UsbDownload_Start\n"));

    if(_bAPBinFlag == TRUE)
    {
        if((_bFlashID1 == TRUE)&&(_bFlashID2 == TRUE))
        {
            printf("== Case 1 ==\n");
            if (_MW_UsbDownload_Copy(_u32DRAMAddr, _u32DRAMLen, _u32FlashSize))
            {

                printf("_MW_UsbDownload_Copy done, _u32DRAMAddr=0x%x, _u32DRAMLen=0x%x, _u32FlashSize=0x%x\n", _u32DRAMAddr, _u32DRAMLen, _u32FlashSize);
                while(1);

                #if (ENABLE_6M30_3D_PROCESS)
                  if (MDrv_Ursa_USB_Update_GetChipType() == SWUpdateForMFC)
                  {
                        if(MApp_Ursa_SwUpdate_Start())
                        {
                            USB_DL_ASSERT(printf("Error> Update the 1st flash failed\n"));
                        }
                        else
                        {
                            USB_DL_DBG(printf("Update the 1st flash successfully\n"));
                        }
                  }
           else
               #endif
                  {
                        if(MApp_SwUpdate_Start((EN_SW_UPDATE_MODE)_u8UpgradeMode, FALSE, FALSE, TRUE, _u32DRAMAddr, _u32DownloadSize, _u8MemType, TRUE) == FALSE)
                        {
                            USB_DL_ASSERT(printf("Error> Update the 1st flash failed\n"));
                        }
                        else
                        {
                            USB_DL_DBG(printf("Update the 1st flash successfully\n"));
                        }
                  }
            }
            else
            {
                USB_DL_ASSERT(printf("Error> Copy Merge.bin failed\n"));

                return FALSE;
            }

            _MW_UsbDownload_CheckHeader();
            _MW_UsbDownload_Exit();
            return MApp_SwUpdate_Start((EN_SW_UPDATE_MODE)_u8UpgradeMode, TRUE, TRUE, FALSE, _u32DRAMAddr, _u32DownloadSize, _u8MemType, TRUE);
        }
        else if((_bFlashID1 == TRUE)&&(_bFlashID2 == FALSE))
        {
            printf("== Case 2 ==\n");
            if (_MW_UsbDownload_Copy(_u32DRAMAddr, _u32DRAMLen, _u32FlashSize))
            {
                _MW_UsbDownload_Exit();
                return MApp_SwUpdate_Start((EN_SW_UPDATE_MODE)_u8UpgradeMode, FALSE, FALSE, FALSE, _u32DRAMAddr, _u32DownloadSize, _u8MemType, TRUE);
            }
            else
            {
                USB_DL_ASSERT(printf("Error> Copy Merge.bin failed\n"));
            }
        }
        else if((_bFlashID1 == FALSE)&&(_bFlashID2 == TRUE))
        {
            printf("== Case 3 ==\n");
            if(_MW_UsbDownload_CheckHeader())
            {
                _MW_UsbDownload_Exit();
                return MApp_SwUpdate_Start((EN_SW_UPDATE_MODE)_u8UpgradeMode, TRUE, TRUE, FALSE, _u32DRAMAddr, _u32DownloadSize, _u8MemType, TRUE);
            }
            else
            {
                USB_DL_ASSERT(printf("Error> Copy Merge2.bin failed\n"));
            }
        }
    }
    else
    {
        if (_MW_UsbDownload_Copy(_u32DRAMAddr, _u32DRAMLen, _u32FlashSize))
        {
            printf("== Case 4 ==\n");

            _MW_UsbDownload_Exit();
#if ENABLE_USB_HOST_DUAL_IMAGE
            bFullDL = bFullDL;

            u32DualImgOffset = MW_UsbDownload_GetDualImageOffset();
            USB_DL_DBG(printf("USB download offset of dual image: 0x%x\n",u32DualImgOffset));
#else
            if(!bFullDL)
            {
                u32DownloadOffset  = msRead4Byte(REG_0010F6); // PM.bin start
                u32DownloadOffset -= 0x80; // sboot.bin end, 0x80:chunk header size

                if( (u32DownloadOffset&BIT31) || (u32DownloadOffset==0)
                  ||(u32DownloadOffset&0x0000FFFF) || (u32DownloadOffset>_u32FileSize) ) // negative or sboot not 64KB align
                {
                    printf("Error> USB partial download offset incorrect\n");
                    return FALSE;
                }
            }
#endif   
            printf("u32DownloadOffset = 0x%x\n",u32DownloadOffset);

            FlashUpdateFW(TRUE, u32DownloadOffset + u32DualImgOffset, (BYTE*)(_u32DRAMAddr+u32DownloadOffset), _u32FileSize-u32DownloadOffset, FALSE);

            return FALSE;

          #if (ENABLE_6M30_3D_PROCESS)
            if (MDrv_Ursa_USB_Update_GetChipType() == SWUpdateForMFC)
            {
                printf("== Case 5 ==\n");
                if(MApp_Ursa_SwUpdate_Start())
                {
                    USB_DL_ASSERT(printf("Error> Update the 1st flash failed\n"));
                    return TRUE;
                }
                else
                {
                    USB_DL_DBG(printf("Update the 1st flash successfully\n"));
                    return FALSE;
                }
            }
            else
          #endif
            {
                USB_DL_DBG( printf(" _u8UpgradeMode=%u, _u8FlashID=%u, _u32DRAMAddr=0x%lx, _u32DownloadSize=0x%lx, _u8MemType=%u\n",  _u8UpgradeMode, _u8FlashID, _u32DRAMAddr, _u32DownloadSize, _u8MemType); );
                return MApp_SwUpdate_Start((EN_SW_UPDATE_MODE)_u8UpgradeMode, _u8FlashID, FALSE, FALSE, _u32DRAMAddr, _u32DownloadSize, _u8MemType, FALSE);
            }
        }
    }
    return FALSE;
}

#if (ENABLE_SZ_FACTORY_USB_SAVE_DATABASE_FUNCTION == ENABLE ||ENABLE_SZ_FACTORY_USB_SAVE_MAP_FUNCTION == ENABLE)
void MApp_UsbSaveData_SetPort(U8 u8USBPortNo)
{
    _u8USBPortNo = u8USBPortNo;
}

U8 MApp_UsbSaveData_OpenNewFileForWrite(U16* pString,U8 charLen)
{
    return msAPI_FCtrl_FileCreate(_u8USBEnvironmentID ,pString, charLen, CREATE_MODE_NEW);
}

BOOLEAN MApp_UsbSaveData_SearchFileInRoot(U8* pu8FileName, FileEntry* pFileEntry)
{
    return _MW_UsbDownload_SearchFileInRoot(pu8FileName, pFileEntry);
}

BOOLEAN MApp_UsbSaveData_InitFileSystem(void)
{
    return _MW_UsbDownload_InitFileSystem();
}

void MApp_UsbSaveData_Exit(void)
{
    _MW_UsbDownload_Exit();
}
#endif


#if( ENABLE_USB_DOWNLOAD_BIN )

#define DEBUG_USB_DL_BIN(x)   x


#include "msAPI_Memory.h"


static BOOL MW_EraseFlash(U32 u32DestFlashAddr, U32 u32Size)
{
    if( (u32DestFlashAddr&0xFFFF) != 0 )
    {
        printf("\n Error: Invalid u32DestFlashAddr=0x%lX\n", u32DestFlashAddr);
        return FALSE;
    }

    if( (u32Size&0xFFFF) != 0 )
    {
        printf("\n Error: Invalid u32Size=0x%lX\n", u32Size);
        return FALSE;
    }

    if( FALSE == MDrv_FLASH_AddressErase(u32DestFlashAddr, u32Size, TRUE) )
    {
        printf("\n Error: MDrv_FLASH_AddressErase() failed!\n");
        return FALSE;
    }

    {   // Check flash erase status
        U16 u16FlashBlockStart = u32DestFlashAddr >> 16;
        U16 u16FlashBlockEnd = (u32DestFlashAddr + u32Size - 1) >> 16;
        U16 u16BlockIdx;
        U32 i;
        U8 au8FlashBuf[16];

        for( u16BlockIdx = u16FlashBlockStart; u16BlockIdx <= u16FlashBlockEnd; ++ u16BlockIdx )
        {
            MDrv_FLASH_Read(u32DestFlashAddr, 16, au8FlashBuf );
            for( i = 0; i < 16; ++ i )
            {
                if( au8FlashBuf[i] != 0xFF )
                {
                    printf("\n Error: Flash blanking failed! addr=0x%lX\n", u32DestFlashAddr + i );
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}

EnuUsbDlErrCode MW_CopyDramToFlash(U32 u32SrcDramAddr, U32 u32DestFlashAddr, U32 u32Size)
{
    EnuUsbDlErrCode errCode = E_USB_DL__OK;
    U8 *flash_buf;
    U32 u32CopySize;
    //BOOL bResult = TRUE;
    BOOL bNeedEnableWriteProtect = FALSE;


#define DRAM_TO_FLASH_BUF   1024


    DEBUG_USB_DL_BIN( printf("MW_CopyDramToFlash()\n"); );

    //msAPI_Flash_ChipSelect((FLASH_CHIP_SELECT)0);

    if( (u32DestFlashAddr&0xFFFF) != 0 )
    {
        printf("\n Error: Invalid u32DestFlashAddr=0x%lX\n", u32DestFlashAddr);
        return E_USB_DL__PARAMETER_ERROR;
    }

    if( (u32Size&0xFFFF) != 0 )
    {
        printf("\n Error: Invalid u32Size=0x%lX\n", u32Size);
        return E_USB_DL__PARAMETER_ERROR;
    }

    flash_buf = (U8*)msAPI_Memory_Allocate(DRAM_TO_FLASH_BUF, BUF_ID_USB_DOWNLOAD_BIN);
    if (flash_buf == NULL)
    {
        //__ASSERT(0);
        printf("\nmalloc memory fail!\n");
        return E_USB_DL__MEMORY_ALLOC_FAILED;
    }


    do
    {
        if( FALSE == MDrv_FLASH_WriteProtect_Disable_Range_Set(u32DestFlashAddr, u32Size) )
        {
            printf("\nMDrv_FLASH_WriteProtect_Disable_Range_Set() failed!\n");
            //bResult = FALSE;
            errCode = E_USB_DL__FLASH_WRITE_PROTECT_FAILED;
            break;
        }

        bNeedEnableWriteProtect = TRUE;

        if( FALSE == MW_EraseFlash( u32DestFlashAddr, u32Size) )
        {
            //bResult = FALSE;
            errCode = E_USB_DL__ERASE_FLASH_FAILED;
            break;
        }

        // Copy dram to buf, then copy buf to flash
        MsOS_FlushMemory();
        while( u32Size > 0 )
        {
            u32CopySize = MIN(u32Size, DRAM_TO_FLASH_BUF);

            // Copy new data to flash_buf
            memcpy( (flash_buf), (void *)_PA2VA(u32SrcDramAddr),  u32CopySize);

            // Copy flash_buf to flash
            MDrv_FLASH_Write(u32DestFlashAddr, u32CopySize, flash_buf);

            u32SrcDramAddr += u32CopySize;
            u32DestFlashAddr += u32CopySize;
            u32Size -= u32CopySize;
        }
    }while(0);


    if( bNeedEnableWriteProtect )
    {
        MDrv_FLASH_WriteProtect(ENABLE);
    }

    if( flash_buf )
    {
        MSAPI_MEMORY_FREE(flash_buf, BUF_ID_USB_DOWNLOAD_BIN);
    }

    return errCode;
}


void MW_UsbDlBin_SearchFile_Exit(void)
{
    if( !IsStorageInUse() )
        _MW_UsbDownload_Exit();
}

EnuUsbDlErrCode MW_UsbDlBin_SearchFileAndCopyToDram(U8 u8USBPortNo, U8* pu8FileName, U32* pu32FileSize, U32 u32DramAddr, U32 u32DramSize )
{
    FileEntry fileEntry;
    U32 u32FileSize;
    U8 u8HandleNo;
    U32 u32LengthLeft = 0;


    // Search PQ bin file in USB...
    DEBUG_USB_DL_BIN( printf("MW_UsbDlBin_SearchFileAndCopyToDram()\n"); );


    _u8USBPortNo = u8USBPortNo;
    if( FALSE == _MW_UsbDownload_InitFileSystem() )
    {
        printf("\n Error: _MW_UsbDownload_InitFileSystem() failed!\n");
        return E_USB_DL__INIT_FILE_SYSTEM_FAILED;
    }

    if( FALSE == _MW_UsbDownload_SearchFileInRoot(pu8FileName, &fileEntry) )
    {
        printf("\n Error: Search file failed!\n");
        return E_USB_DL__SEARCH_FILE_FAILED;
    }
    else
    {
        //printf(" Search file OK~\n");
    }

    MApp_UsbClearWatchDog();


    u8HandleNo = msAPI_FCtrl_FileOpen( &fileEntry, OPEN_MODE_FOR_READ );
    if( u8HandleNo == FCTRL_INVALID_FILE_HANDLE)
    {
        printf("\n Error: Open file failed\n");
        MW_UsbDlBin_SearchFile_Exit();
        return E_USB_DL__OPEN_FILE_FAILED;
    }


    // Copy file to dram ...
    *pu32FileSize = u32FileSize = msAPI_FCtrl_FileLength(u8HandleNo);
    DEBUG_USB_DL_BIN( printf(" File size: %lu\n", u32FileSize); );
    DEBUG_USB_DL_BIN( printf(" Dram Addr: 0x%lx\n", u32DramAddr); );
    DEBUG_USB_DL_BIN( printf(" Dram Size: 0x%lx\n", u32DramSize); );

    if( (u32FileSize == 0) || (u32FileSize > u32DramSize) )
    {
        printf("\n Error: File size=%lu error!\n", u32FileSize);
        msAPI_FCtrl_FileClose(u8HandleNo);
        MW_UsbDlBin_SearchFile_Exit();
        return E_USB_DL__FILE_SIZE_IS_ERROR;
    }

    MApp_UsbClearWatchDog();

    DEBUG_USB_DL_BIN( printf(" Copying file from USB to DRAM\n"); );
    u32LengthLeft = msAPI_FCtrl_FileRead(u8HandleNo, u32DramAddr, u32FileSize);
    MsOS_FlushMemory();
    msAPI_FCtrl_FileClose(u8HandleNo);

    if( u32LengthLeft != 0 )
    {
        printf("\n Error: Copy file to dram failed!\n");
        MW_UsbDlBin_SearchFile_Exit();
        return E_USB_DL__READ_FILE_FAILED;
    }

    MW_UsbDlBin_SearchFile_Exit();

    return E_USB_DL__OK;
}

void MW_Miu_ClearProtectHitFlag(void)
{
    MDrv_WriteByteMask(0x1012DE, 0x01, 0x01); /* Clear hit bit */
    MDrv_WriteByteMask(0x1012DE, 0x00, 0x01);
}

BOOL MW_Miu_IsInvalidAccessHappened(U16 u16Flag)
{
    if( MDrv_ReadByte(0x1012DE)&0x10 )
    {
        printf("== Miu Invalid access happened!! u16Flag=%u ==\n", u16Flag);
        printf(" hit_protect_no=%d, hit_protect_id=%x\n", (MDrv_ReadByte(0x1012DE)&0xE0)>>5, (MDrv_ReadByte(0x1012DF)&0x3F) ); \
        printf("==============================================\n");
        MDrv_WriteByteMask(0x1012DE, 0x01, 0x01); /* Clear hit bit */
        MDrv_WriteByteMask(0x1012DE, 0x00, 0x01);
        return TRUE;
    }
    return FALSE;
}

#define ENABLE_CHECK_MIU_PROTECT    0
EnuUsbDlErrCode MW_Usb_Download_Bin(U8 u8USBPortNo, U8* pu8FileName, U32 u32DramBufAddr, U32 u32DramBufSize, U16 u16BinId)
{
    EnuUsbDlErrCode usbDlErrCode = E_USB_DL__OK;

    U32 u32FileSize = 0;

    U32 u32DramDownloadBufAddr1 = u32DramBufAddr;
    U32 u32DramDownloadBufAddr2 = u32DramDownloadBufAddr1 + 0x100000;
    U32 u32BinDataDramAddr;

    U32 u32Bin_FlashStart, u32Bin_FlashSize, u32Bin_FlashEnd;
    U32 u32FlashCopy_Start, u32FlashCopy_Size;

    BDMA_Result bdma_Result;


    printf("MW_Usb_Download_Bin(u8USBPortNo=%u, pu8FileName=%s, u16BinId=0x%X)\n",
                                    u8USBPortNo, pu8FileName, u16BinId);

    printf(" Dram buf: start=0x%lx, size=0x%lx\n", u32DramBufAddr, u32DramBufSize);

    if( u32DramBufSize < 0x200000 )
    {
        printf("\n Error: Dram buffer size(0x%lu) is small then 0x200000!\n", u32DramBufSize);
        return E_USB_DL__MEMORY_BUFFER_SIZE_ERROR;
    }

#if( ENABLE_CHECK_MIU_PROTECT )
    MW_Miu_ClearProtectHitFlag();
    MsOS_DelayTask(5);

    if( MW_Miu_IsInvalidAccessHappened(__LINE__) )
    {
        printf("\n Error: Miu Invalid Access Happened!\n");
        return E_USB_DL__MEMORY_PROTECT_HIT_HAPPEN;
    }
#endif

    // Search file in USB and copy file data to dram buf-1
    printf(" Search file in USB and copy file data to dram buf-1(0x%lX)\n", u32DramDownloadBufAddr1);
    usbDlErrCode = MW_UsbDlBin_SearchFileAndCopyToDram( u8USBPortNo, pu8FileName, &u32FileSize, u32DramDownloadBufAddr1, 0x100000 );
    if( usbDlErrCode != E_USB_DL__OK )
    {
        //printf("\n Error: Search file failed!\n");
        return usbDlErrCode;
    }
    else
    {
        printf("  Search file OK~\n");
    }

#if( ENABLE_CHECK_MIU_PROTECT )
    if( MW_Miu_IsInvalidAccessHappened(__LINE__) )
    {
        printf("\n Error: Miu Invalid Access Happened!\n");
        return E_USB_DL__MEMORY_PROTECT_HIT_HAPPEN;
    }
#endif

    // Get bin info...
    printf(" Get bin info\n");
    {
        BININFO binInfo;
        BOOLEAN bResult;

        binInfo.B_ID = u16BinId;
        MDrv_Sys_Get_BinInfo(&binInfo, &bResult);
        if (bResult != PASS)
        {
            printf("\n Error: Could not find bin(0x%X) on flash.\n", u16BinId);
            return E_USB_DL__GET_BIN_INFO_FAILED;
        }

        u32Bin_FlashStart = binInfo.B_FAddr;
        u32Bin_FlashSize = binInfo.B_Len;
        u32Bin_FlashEnd = u32Bin_FlashStart + u32Bin_FlashSize - 1;
        printf(" Bin-Flash: start=%lu(0x%lX), size=%lu(0x%lX)\n",
            u32Bin_FlashStart, u32Bin_FlashStart, u32Bin_FlashSize, u32Bin_FlashSize);
    }

    // Check file and bin size...
    if( u32FileSize > u32Bin_FlashSize )
    {
        printf("\nError: File size(%lu) is big then bin size(%lu)!\n", u32FileSize, u32Bin_FlashSize);
        return E_USB_DL__FILE_SIZE_IS_BIG_THEN_BIN_SIZE;
    }

    // Check flash copy range
    {
        U32 u32FlashBankStart = u32Bin_FlashStart >> 16;
        U32 u32FlashBankEnd = u32Bin_FlashEnd >> 16;

        printf(" u32FlashBankStart=0x%X, u32FlashBankEnd=0x%X\n",
            u32FlashBankStart, u32FlashBankEnd );

        u32FlashCopy_Start = u32FlashBankStart << 16;
        u32FlashCopy_Size = (u32FlashBankEnd-u32FlashBankStart+1)*0x10000;
        printf("  FlashCopy: start=%lu(0x%lX), size=%lu(0x%lX)\n",
                    u32FlashCopy_Start, u32FlashCopy_Start, u32FlashCopy_Size, u32FlashCopy_Size);
    }

#if( ENABLE_CHECK_MIU_PROTECT )
    if( MW_Miu_IsInvalidAccessHappened(__LINE__) )
    {
        printf("\n Error: Miu Invalid Access Happened!\n");
        return E_USB_DL__MEMORY_PROTECT_HIT_HAPPEN;
    }
#endif

    // Copy flash bin data  to dram buf-2
    printf(" Copy flash bin data to dram:\n");
    printf("  Dram Addr-2 = 0x%lX\n", u32DramDownloadBufAddr2);
    bdma_Result = MApi_BDMA_Copy(u32FlashCopy_Start, u32DramDownloadBufAddr2, u32FlashCopy_Size, MIU_FLASH2SDRAM);
    if( E_BDMA_OK != bdma_Result )
    {
        printf("MApi_BDMA_Copy failed! BDMA_Result=%u\n", bdma_Result);
        return E_USB_DL__BDMA_FLASH_TO_DRAM_FAILED;
    }
    MsOS_FlushMemory();

#if( ENABLE_CHECK_MIU_PROTECT )
    if( MW_Miu_IsInvalidAccessHappened(__LINE__) )
    {
        printf("\n Error: Miu Invalid Access Happened!\n");
        return E_USB_DL__MEMORY_PROTECT_HIT_HAPPEN;
    }
#endif

    // Update bin data in dram by file data
    printf(" Update bin data in dram buf-2 by file data\n");

    u32BinDataDramAddr = u32DramDownloadBufAddr2 + (u32Bin_FlashStart&0xFFFF);

    printf("  Bin data dram addr = 0x%lX\n", u32BinDataDramAddr);

#if 1
    // Clear old bin data
    {
        U8 *pu8PQData;
        pu8PQData = (U8*)_PA2VA(u32BinDataDramAddr);
        memset( pu8PQData, 0, u32Bin_FlashSize );
        MsOS_FlushMemory();
    }
#endif

#if( ENABLE_CHECK_MIU_PROTECT )
    if( MW_Miu_IsInvalidAccessHappened(__LINE__) )
    {
        printf("\n Error: Miu Invalid Access Happened!\n");
        return E_USB_DL__MEMORY_PROTECT_HIT_HAPPEN;
    }
#endif

    // Copy file data from dram buf-1to dram buf-2
    bdma_Result = MApi_BDMA_Copy(u32DramDownloadBufAddr1, u32BinDataDramAddr, u32FileSize, MIU_SDRAM2SDRAM);
    if( E_BDMA_OK != bdma_Result )
    {
        printf("\n Error: MApi_BDMA_Copy failed! BDMA_Result=%u\n", bdma_Result);
        return E_USB_DL__BDMA_FLASH_TO_DRAM_FAILED;
    }
    MsOS_FlushMemory();

#if( ENABLE_CHECK_MIU_PROTECT )
    if( MW_Miu_IsInvalidAccessHappened(__LINE__) )
    {
        printf("\n Error: Miu Invalid Access Happened!\n");
        return E_USB_DL__MEMORY_PROTECT_HIT_HAPPEN;
    }
#endif

    // Copy dram data to flash
    printf(" Copy dram data to flash: DramAddr=0x%lx FlashAddr=0x%lx Size=0x%lx\n", u32DramDownloadBufAddr2, u32FlashCopy_Start, u32FlashCopy_Size);
    usbDlErrCode = MW_CopyDramToFlash(u32DramDownloadBufAddr2, u32FlashCopy_Start, u32FlashCopy_Size);
    if( E_USB_DL__OK != usbDlErrCode )
    {
        printf("\n Error: MW_CopyDramToFlash failed!\n");
        return usbDlErrCode;
    }

#if( ENABLE_CHECK_MIU_PROTECT )
    if( MW_Miu_IsInvalidAccessHappened(__LINE__) )
    {
        printf("\n Error: Miu Invalid Access Happened!\n");
        return E_USB_DL__MEMORY_PROTECT_HIT_HAPPEN;
    }
#endif

    printf(" Done~\n");

    return E_USB_DL__OK;
}


#endif

#if (ENABLE_USB_HOST_DUAL_IMAGE)
#define CHUNK_HEADER_SIZE           0x80
#define SBOOT_SIZE_START_ADDRESS     0x24    //0x24~0x27: sboot.bin size
#define BIN_SIZE_START_ADDRESS       0x20    //0x20~0x23: merge.bin size

U32 MW_UsbDownload_GetDualImageOffset(void)
{
   U32 u32DualImgTmp;
   U32 u32DualImgOffset;
   
   u32DualImgTmp = msRead4Byte(REG_0010F6); // PM.bin start of dual image
   u32DualImgTmp -= CHUNK_HEADER_SIZE; // sboot.bin end, 0x80:chunk header size

   Flash_ReadTbl(u32DualImgTmp + SBOOT_SIZE_START_ADDRESS, (BYTE*)&u32DualImgOffset, 4); //get sboot.bin size
   u32DualImgTmp = u32DualImgOffset;
   Flash_ReadTbl(u32DualImgTmp + BIN_SIZE_START_ADDRESS, (BYTE*)&u32DualImgOffset, 4); //get merge.bin size

   USB_DL_DBG(printf("bin file size before align:0x%x\n",u32DualImgOffset));

   u32DualImgOffset = ALIGN_64K(u32DualImgOffset + 4); // 4 bytes CRC

   return u32DualImgOffset;
}
#endif

#undef MW_USBDOWNLOAD_C

