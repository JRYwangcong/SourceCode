/*
 * Copyright (c) 2013 MStar Semiconductor, Inc.
 * All rights reserved.
 *
 * monitor_ap/kernel/system/xmodem.c
 *
 * Date:        08-Oct-2013
 * Comment:
 *       XMODEM protocol subroutines.
 * Reversion History:
 */

#define _XMODEM_C_
#include "sysinfo.h"
#include "datatype.h"
#include "Board.h"
#include "xmodem.h"
#include "misc.h"
#include "msflash.h"
#include "ms_reg_MST9U.h"
#include "Ms_rwreg.h"

/* XMODEM protocol commands */
#define XMODEM_SOH 0x01
#define XMODEM_STX 0x02
#define XMODEM_EOT 0x04
#define XMODEM_ACK 0x06
#define XMODEM_NAK 0x15
#define XMODEM_CAN 0x18
#define XMODEM_ESC 0x1b

#define XMODEM_PACKET_DATA_SIZE     128
#define XMODEM_1K_PACKET_DATA_SIZE  1024
#define XMODEM_PACKET_HEADER_SIZE   3
#if XMODEM_CRC_ENABLE
#define XMODEM_PACKET_CHECK_SIZE    2
#else
#define XMODEM_PACKET_CHECK_SIZE    1
#endif

/* Maximum downloaded file size (2MB) */
//#define XMODEM_DWNLD_FILE_SIZE (2*1024*1024)

/* Delay for waiting a user to open the sending file (10 s) */
#define XMODEM_WAIT_USER_DELAY (10000)
/* Delay for resend the XMODEM start character (3s) */
#define XMODEM_RESEND_START_CHAR_DELAY (3000)

/* Variables for storing the content of a packet */
unsigned char xmodem_packet_start_byte;
unsigned char xmodem_packet_number;
unsigned char xmodem_packet_number_complement;
#if XMODEM_CRC_ENABLE
unsigned char xmodem_packet_crc_high;
unsigned char xmodem_packet_crc_low;
#else
unsigned char xmodem_packet_checksum;
#endif

/* Flags for controlling the XMODEM protocol flow */
volatile bool xmodem_start_flag;
volatile bool xmodem_packet_received_flag;

/* Caculated checksum or CRC-16 */
unsigned short xmodem_calculated_check_value;

/* Buffer for receiving the downloading file and index of the buffer */
//unsigned char xmodem_recv_buffer[XMODEM_DWNLD_FILE_SIZE];
unsigned char *xmodem_recv_buffer = (unsigned char *)DOWNLOAD_BUFFER_ADR;
unsigned int xmodem_recv_buffer_index;

#if XMODEM_CRC_ENABLE
static unsigned short crctab[256] = {
    0x0000,  0x1021,  0x2042,  0x3063,  0x4084,  0x50a5,  0x60c6,  0x70e7,
    0x8108,  0x9129,  0xa14a,  0xb16b,  0xc18c,  0xd1ad,  0xe1ce,  0xf1ef,
    0x1231,  0x0210,  0x3273,  0x2252,  0x52b5,  0x4294,  0x72f7,  0x62d6,
    0x9339,  0x8318,  0xb37b,  0xa35a,  0xd3bd,  0xc39c,  0xf3ff,  0xe3de,
    0x2462,  0x3443,  0x0420,  0x1401,  0x64e6,  0x74c7,  0x44a4,  0x5485,
    0xa56a,  0xb54b,  0x8528,  0x9509,  0xe5ee,  0xf5cf,  0xc5ac,  0xd58d,
    0x3653,  0x2672,  0x1611,  0x0630,  0x76d7,  0x66f6,  0x5695,  0x46b4,
    0xb75b,  0xa77a,  0x9719,  0x8738,  0xf7df,  0xe7fe,  0xd79d,  0xc7bc,
    0x48c4,  0x58e5,  0x6886,  0x78a7,  0x0840,  0x1861,  0x2802,  0x3823,
    0xc9cc,  0xd9ed,  0xe98e,  0xf9af,  0x8948,  0x9969,  0xa90a,  0xb92b,
    0x5af5,  0x4ad4,  0x7ab7,  0x6a96,  0x1a71,  0x0a50,  0x3a33,  0x2a12,
    0xdbfd,  0xcbdc,  0xfbbf,  0xeb9e,  0x9b79,  0x8b58,  0xbb3b,  0xab1a,
    0x6ca6,  0x7c87,  0x4ce4,  0x5cc5,  0x2c22,  0x3c03,  0x0c60,  0x1c41,
    0xedae,  0xfd8f,  0xcdec,  0xddcd,  0xad2a,  0xbd0b,  0x8d68,  0x9d49,
    0x7e97,  0x6eb6,  0x5ed5,  0x4ef4,  0x3e13,  0x2e32,  0x1e51,  0x0e70,
    0xff9f,  0xefbe,  0xdfdd,  0xcffc,  0xbf1b,  0xaf3a,  0x9f59,  0x8f78,
    0x9188,  0x81a9,  0xb1ca,  0xa1eb,  0xd10c,  0xc12d,  0xf14e,  0xe16f,
    0x1080,  0x00a1,  0x30c2,  0x20e3,  0x5004,  0x4025,  0x7046,  0x6067,
    0x83b9,  0x9398,  0xa3fb,  0xb3da,  0xc33d,  0xd31c,  0xe37f,  0xf35e,
    0x02b1,  0x1290,  0x22f3,  0x32d2,  0x4235,  0x5214,  0x6277,  0x7256,
    0xb5ea,  0xa5cb,  0x95a8,  0x8589,  0xf56e,  0xe54f,  0xd52c,  0xc50d,
    0x34e2,  0x24c3,  0x14a0,  0x0481,  0x7466,  0x6447,  0x5424,  0x4405,
    0xa7db,  0xb7fa,  0x8799,  0x97b8,  0xe75f,  0xf77e,  0xc71d,  0xd73c,
    0x26d3,  0x36f2,  0x0691,  0x16b0,  0x6657,  0x7676,  0x4615,  0x5634,
    0xd94c,  0xc96d,  0xf90e,  0xe92f,  0x99c8,  0x89e9,  0xb98a,  0xa9ab,
    0x5844,  0x4865,  0x7806,  0x6827,  0x18c0,  0x08e1,  0x3882,  0x28a3,
    0xcb7d,  0xdb5c,  0xeb3f,  0xfb1e,  0x8bf9,  0x9bd8,  0xabbb,  0xbb9a,
    0x4a75,  0x5a54,  0x6a37,  0x7a16,  0x0af1,  0x1ad0,  0x2ab3,  0x3a92,
    0xfd2e,  0xed0f,  0xdd6c,  0xcd4d,  0xbdaa,  0xad8b,  0x9de8,  0x8dc9,
    0x7c26,  0x6c07,  0x5c64,  0x4c45,  0x3ca2,  0x2c83,  0x1ce0,  0x0cc1,
    0xef1f,  0xff3e,  0xcf5d,  0xdf7c,  0xaf9b,  0xbfba,  0x8fd9,  0x9ff8,
    0x6e17,  0x7e36,  0x4e55,  0x5e74,  0x2e93,  0x3eb2,  0x0ed1,  0x1ef0
};

#define updcrc(ch, crc) (crctab[((crc >> 8) & 255) ^ ch] ^ (crc << 8))
#endif

int xmodem_send_start_char(void);
int xmodem_check_packet(void);

void xmodem_init()
{
    xmodem_mode = XMODEM_NON_ACTIVE;
    xmodem_start_flag = FALSE;
    xmodem_packet_received_flag = FALSE;
    xmodem_calculated_check_value = 0;
}

/**
 * xmodem_packet_receiver - receive a packet from UART RX
 * @c: the current byte being received
 *
 * Receive a byte at a time from the UART RX and set
 * xmodem_packet_received_flag true until receive a full packet.
 *
 * This function is called by MDrv_UART_RecvHandler which is
 * the UART RX callback function.
 */
void xmodem_packet_receiver(int c)
{
    /* Number of bytes received in the current packet */
    static unsigned short num_bytes_received = 0;
    /* The size of a packet and the size of data of a packet in bytes */
    static unsigned short packet_size = 0;
    static unsigned short data_size = 0;

    if (num_bytes_received == 0)
    {
        /* Store the first byte (default: SOH or STX) of a packet. */
        xmodem_packet_start_byte = c;

        if (c == XMODEM_SOH)        /* The sender is sending a XMODEM packet. */
            data_size = XMODEM_PACKET_DATA_SIZE;
        else if (c == XMODEM_STX)   /* The sender is sending a XMODEM-1K packet. */
            data_size = XMODEM_1K_PACKET_DATA_SIZE;

        packet_size = data_size + XMODEM_PACKET_HEADER_SIZE + XMODEM_PACKET_CHECK_SIZE;

        /* After receiving SOH or STX of the first packet, set this flag TRUE
           to stop xmodem_start_transfer keeping sending 'C' or NAK. */
        if ((xmodem_start_flag == FALSE) && ((c == XMODEM_SOH) || (c == XMODEM_STX)))
            xmodem_start_flag = TRUE;

        /* End or CANCEL of transfer */
        if ((c == XMODEM_CAN) || (c == XMODEM_ESC) || (c == XMODEM_EOT))
        {
            xmodem_packet_received_flag = TRUE;
            num_bytes_received = 0;
            return;
        }
    }
    else if (num_bytes_received == 1)
    {
        xmodem_packet_number = c;
    }
    else if (num_bytes_received == 2)
    {
        xmodem_packet_number_complement = c;
    }
    else if (num_bytes_received < (XMODEM_PACKET_HEADER_SIZE + data_size))  /* From 3th to 130th or 1026th bytes */
    {
        xmodem_recv_buffer[xmodem_recv_buffer_index + num_bytes_received - XMODEM_PACKET_HEADER_SIZE] = c;

        /* Caculate CRC-16 or checksum */
        #if (XMODEM_CRC_ENABLE)
        xmodem_calculated_check_value = updcrc(c, xmodem_calculated_check_value);
        #else
        xmodem_calculated_check_value = (xmodem_calculated_check_value + c) & 0x00FF;
        #endif
    }
    else if (num_bytes_received < packet_size)  /* Receive checksum or CRC-16 */
    {
        #if (XMODEM_CRC_ENABLE == 0)
        xmodem_packet_checksum = c;
        #else
        if (num_bytes_received == (packet_size - XMODEM_PACKET_CHECK_SIZE))
            xmodem_packet_crc_high = c;
        else
            xmodem_packet_crc_low = c;
        #endif
    }

    num_bytes_received++;
    if (num_bytes_received == packet_size)      /* Receive a full packet */
    {
        xmodem_packet_received_flag = TRUE;
        num_bytes_received = 0;
    }
}

int xmodem_send_start_char()
{
    unsigned char retry = 10;

    printf("\033[2J\033[1;1H");                 /* Clean hyperterminal screen */
    printf("[XMODEM] Wait for starting transfer...");

    xmodem_recv_buffer_index = 0;

    ForceDelay1ms(XMODEM_WAIT_USER_DELAY);

    while (retry--)
    {
        switch (xmodem_mode)
        {
            case XMODEM:
                #if XMODEM_CRC_ENABLE
                putchar('C');
                #else
                putchar(XMODEM_NAK);
                #endif
                break;
            default:
                putchar(XMODEM_NAK);
                break;
        }

        /* Wait and test if xmodem_packet_receiver received SOH or STX of
           the first packet. If not, resend the start chracter. */
        ForceDelay1ms(XMODEM_RESEND_START_CHAR_DELAY);
        if (xmodem_start_flag == TRUE)
            break;

        if (retry == 0)         /* Fail to start xmodem transfer */
            return  EXMSTART;
    }

    return XMREADY;             /* Ready for receving the first packet */
}

int xmodem_check_packet()
{
    /* Reset the flag */
    xmodem_packet_received_flag = FALSE;

    /* Receive the last packet: end of transfer. */
    if (xmodem_packet_start_byte == XMODEM_EOT)
    {
        putchar(XMODEM_ACK);
        return XMSUCCESS;       /* Transfer succeed */
    }

    if ((xmodem_packet_start_byte == XMODEM_CAN) || (xmodem_packet_start_byte == XMODEM_ESC))
        return EXMCANCEL;       /* Transfer is canceled */

    /* Receive a normal packet */
    if ((xmodem_packet_start_byte == XMODEM_SOH) || (xmodem_packet_start_byte == XMODEM_STX))
    {
        bool resend = FALSE;

        /* Check packet number */
        if ((xmodem_packet_number + xmodem_packet_number_complement) != 0xFF)
            resend = TRUE;

        /* Check checksum or CRC-16 */
        #if (XMODEM_CRC_ENABLE)
        if (xmodem_calculated_check_value != ((xmodem_packet_crc_high << 8) + xmodem_packet_crc_low))
            resend = TRUE;
        #else
        if (xmodem_calculated_check_value != xmodem_packet_checksum)
            resend = TRUE;
        #endif
        xmodem_calculated_check_value = 0; /* Reset caculated check value */

        if (resend == FALSE)
        {
            if (xmodem_packet_start_byte == XMODEM_SOH)
                xmodem_recv_buffer_index += XMODEM_PACKET_DATA_SIZE;
            else if (xmodem_packet_start_byte == XMODEM_STX)
                xmodem_recv_buffer_index += XMODEM_1K_PACKET_DATA_SIZE;

            return XMNEXT;      /* Wait next packet */
        }
        else
        {
            return XMRESEND;    /* Ask resending this packet */
        }
    }
    else
    {
        return EXMUNKNOWN;      /* Unknown transfer error */
    }
}

int xmodem_file_download()
{
    int ret;
	
    MsOS_DisableInterrupt(E_INT_IRQ_SC_L0);
	
    if ((ret = xmodem_send_start_char()) < 0)
        goto out;

wait_packet:
    while (!xmodem_packet_received_flag);
    if ((ret = xmodem_check_packet()) > 0)
    {
        if (ret == XMNEXT)
            putchar(XMODEM_ACK);/* Send ACK for ask next packet */
        else if (ret == XMRESEND)
            putchar(XMODEM_NAK);/* Send NAK for ask resending this packet */
        goto wait_packet;
    }

out:
    xmodem_mode = XMODEM_NON_ACTIVE;
    xmodem_start_flag = FALSE;
    xmodem_packet_received_flag = FALSE;
    if (ret == XMSUCCESS)
        printf("\n[XMODEM] File download has been successful!\n");
    else if (ret < 0)
    {
        /* Todo: print error messages according error codes */
        printf("\n[XMODEM] Failed...\n");
    }
	
    MsOS_EnableInterrupt(E_INT_IRQ_SC_L0);

    return ret;
}

#if (XMODEM_DWNLD_ENABLE)
int xmodem_write_flash()
{
    printf("[XMODEM] Total bytes received = %d bytes.\n", xmodem_recv_buffer_index);
    FlashUpdateFW(TRUE, 0x00000000, xmodem_recv_buffer, xmodem_recv_buffer_index, FALSE);
    return 0;
}
#endif

#undef _XMODEM_C_

