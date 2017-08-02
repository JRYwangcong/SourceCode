////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __MS_DECOMPRESS_PRIV_H__
#define __MS_DECOMPRESS_PRIV_H__

#if 0

#define OFFSET_BITS     12
#define LENGTH_BITS     4

#define WINDOW_SIZE     (1 << OFFSET_BITS)

#define MAX_UNCODED     2
#define MAX_CODED       ((1 << LENGTH_BITS) + MAX_UNCODED)

#endif

#define UNCOMPRESSED_LENGTH     (14336UL)

#ifdef __mips__

#define BOOT_LOADER_LENGTH	(0x30000)
#define COMPRESSED_LENGTH_POS	(0xFCAUL)

#define DECOMPRESS_BUF	(0xA1100000UL)
#define DECOMPRESS_BUF_CACHEABLE	(0x81100000UL)
#define MEMORY_POOL_CACHEABLE		(0x82100000UL)

    #define NON_CACHEABLE_TO_CACHEABLE_MASK (0xDFFFFFFF)
#elif defined(__arm__)
    #define MS_BUS_MIU0_0000M     (0x40000000UL)
    #define BOOT_LOADER_LENGTH    (0x30000)
    #define COMPRESSED_LENGTH_POS    (0xFCAUL)

    //jc_db: need to fix cache problem
    #define DECOMPRESS_BUF                 (MS_BUS_MIU0_0000M+(CONFIG_DRAM_SIZE_MB_RELOCATE-16)*0x100000)
    #define DECOMPRESS_BUF_CACHEABLE       (DECOMPRESS_BUF)
    #define MEMORY_POOL_CACHEABLE          (MS_BUS_MIU0_0000M+(CONFIG_DRAM_SIZE_MB_RELOCATE-13)*0x100000)
    #define NON_CACHEABLE_TO_CACHEABLE_MASK (0xFFFFFFFF)
#else

    #define COMPRESSED_LENGTH_POS     (0x114AUL)
    #define UNCOMPRESSED_LENGTH_POS   (0x114EUL)

    #define DECOMPRESS_BUF              (0x01100000UL)
    #define DECOMPRESS_BUF_CACHEABLE    (0x01100000UL)
    #define MEMORY_POOL_CACHEABLE       (0x02100000UL)

    #define NON_CACHEABLE_TO_CACHEABLE_MASK (0xFFFFFFFF)
#endif
#define CRC32_POS	(0x14)

#define MEMORY_POOL_SIZE			(0x0A00000UL)


#if 0

#define ENCODED         0       // encoded string
#define UNCODED         1       // unencoded character

typedef struct encoded_string_t
{
    unsigned int offset;    // offset to start of longest match
    unsigned int length;    // length of longest match
} encoded_string_t;

#define Wrap(value, limit)      (value&0xFFF) //(((value) < (limit)) ? (value) : ((value) - (limit)))

#endif

#endif /* __MS_DECOMPRESS_PRIV_H__ */
