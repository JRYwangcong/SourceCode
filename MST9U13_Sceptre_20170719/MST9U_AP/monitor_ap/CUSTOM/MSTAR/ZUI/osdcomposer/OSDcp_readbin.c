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

#define OSDCP_READBIN_C

/******************************************************************************/
/*                    Header Files                        */
/* ****************************************************************************/
#include "OSDcp_readbin.h"

#include "Board.h"
#include "datatype.h"
#include "drvSERFLASH.h"
#include "msAPI_OCP.h"

/********************************************************************************/
/*                     Macro                    */
/* ******************************************************************************/
/********************************************************************************/
/*                     Local                    */
/* ******************************************************************************/
//static U32  osdcp_output_addr;
//static U32  osdcp_bmp_info_addr;
//static U32  osdcp_text_addr;
/********************************************************************************/
/*                   Functions                                                     */
/********************************************************************************/

void OSDcp_readbin_string_ptr( U8 language, U16 id, U16 *pu16stringbuffer )
{
    msAPI_OCP_ReadBinaryString(language, id, pu16stringbuffer);
}

#undef OSDCP_READBIN_C
