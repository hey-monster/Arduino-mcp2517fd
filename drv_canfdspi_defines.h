/*******************************************************************************
   CAN FD SPI Driver: API Defines Header File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_canfdspi_defines.h

  Summary:
    This header file contains object declarations used in the API.
    This also contains device specific defines.

  Description:
    None.
 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2016 Microchip Technology Inc. and its subsidiaries.  
You may use this software and any derivatives exclusively with Microchip products. 
  
THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  
NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, 
INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, 
AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, 
COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 

IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER 
RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED 
OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED BY LAW, 
MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE 
WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE TERMS.
 *******************************************************************************/
//DOM-IGNORE-END

#ifndef DRV_CANFDSPI_DEFINES_H
#define DRV_CANFDSPI_DEFINES_H

#define SPI_DEFAULT_BUFFER_LENGTH 128   //chaoqian modify

// *****************************************************************************
// Section: Defines
#define CRCBASE    0xFFFF
#define CRCUPPER   1
// *****************************************************************************
//! Reverse order of bits in byte
const uint8_t BitReverseTable256[256] = {
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

//! Look-up table for CRC calculation
const uint16_t crc16_table[256] = {
    0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011, 0x8033, 0x0036, 0x003C, 0x8039,
	0x0028, 0x802D, 0x8027, 0x0022, 0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
    0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041, 0x80C3, 0x00C6, 0x00CC, 0x80C9,
	0x00D8, 0x80DD, 0x80D7, 0x00D2, 0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
    0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1, 0x8093, 0x0096, 0x009C, 0x8099,
	0x0088, 0x808D, 0x8087, 0x0082, 0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
    0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1, 0x01E0, 0x81E5, 0x81EF, 0x01EA,
	0x81FB, 0x01FE, 0x01F4, 0x81F1, 0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
    0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151, 0x8173, 0x0176, 0x017C, 0x8179,
	0x0168, 0x816D, 0x8167, 0x0162, 0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
    0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101, 0x8303, 0x0306, 0x030C, 0x8309,
	0x0318, 0x831D, 0x8317, 0x0312, 0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
    0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371, 0x8353, 0x0356, 0x035C, 0x8359,
	0x0348, 0x834D, 0x8347, 0x0342, 0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
    0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2, 0x83A3, 0x03A6, 0x03AC, 0x83A9,
	0x03B8, 0x83BD, 0x83B7, 0x03B2, 0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
    0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291, 0x82B3, 0x02B6, 0x02BC, 0x82B9,
	0x02A8, 0x82AD, 0x82A7, 0x02A2, 0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
    0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1, 0x8243, 0x0246, 0x024C, 0x8249,
	0x0258, 0x825D, 0x8257, 0x0252, 0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
    0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231, 0x8213, 0x0216, 0x021C, 0x8219,
	0x0208, 0x820D, 0x8207, 0x0202
};

// *****************************************************************************
// *****************************************************************************
// Section: Implementation

// Device selection
#define MCP2517FD

// Revision
//#define REV_A
#define REV_B

// Select ISO/non-ISO CRC
#define ISO_CRC 1

// Before B0 address of filter registers was fixed
#ifdef REV_A
#define FIXED_FILTER_ADDRESS
#endif

// Number of implemented FIFOs
#ifndef FPGA
#define CAN_FIFO_08TO15_IMPLEMENTED
#define CAN_FIFO_16TO31_IMPLEMENTED
#endif

// Number of implemented Filters
#ifndef FPGA
#define CAN_FILTER_08TO15_IMPLEMENTED
#define CAN_FILTER_16TO31_IMPLEMENTED
#endif

// Internal oscillator implemented
#ifdef MCP2520FD
#define CAN_INTERNAL_OSC_PRESENT
#endif

// Restricted Operation Mode implemented
#ifdef REV_B
#define CAN_RESTRICTED_MODE_PRESENT
#endif

// Transmit Queue
#ifdef REV_B
#define CAN_TXQUEUE_IMPLEMENTED
#endif

// Up to A1 silicon we had to multiply user address by 4.
#ifdef REV_A
#define USERADDRESS_TIMES_FOUR
#endif

// Maximum Size of TX/RX Object
#define MAX_MSG_SIZE 76

// Maximum number of data bytes in message
#define MAX_DATA_BYTES 64

// *****************************************************************************
// *****************************************************************************
// Section: Object definitions

//! CAN FIFO Channels

typedef enum {
    CAN_FIFO_CH0,
    CAN_FIFO_CH1,
    CAN_FIFO_CH2,
    CAN_FIFO_CH3,
    CAN_FIFO_CH4,
    CAN_FIFO_CH5,
    CAN_FIFO_CH6,
    CAN_FIFO_CH7,
#ifdef CAN_FIFO_08TO15_IMPLEMENTED
    CAN_FIFO_CH8,
    CAN_FIFO_CH9,
    CAN_FIFO_CH10,
    CAN_FIFO_CH11,
    CAN_FIFO_CH12,
    CAN_FIFO_CH13,
    CAN_FIFO_CH14,
    CAN_FIFO_CH15,
#endif
#ifdef CAN_FIFO_16TO31_IMPLEMENTED
    CAN_FIFO_CH16,
    CAN_FIFO_CH17,
    CAN_FIFO_CH18,
    CAN_FIFO_CH19,
    CAN_FIFO_CH20,
    CAN_FIFO_CH21,
    CAN_FIFO_CH22,
    CAN_FIFO_CH23,
    CAN_FIFO_CH24,
    CAN_FIFO_CH25,
    CAN_FIFO_CH26,
    CAN_FIFO_CH27,
    CAN_FIFO_CH28,
    CAN_FIFO_CH29,
    CAN_FIFO_CH30,
    CAN_FIFO_CH31,
#endif
    CAN_FIFO_TOTAL_CHANNELS
} CAN_FIFO_CHANNEL;

#ifdef CAN_TXQUEUE_IMPLEMENTED
#define CAN_FIFO_FIRST_CHANNEL  CAN_FIFO_CH1
#define CAN_TXQUEUE_CH0         CAN_FIFO_CH0
#else
#define CAN_FIFO_FIRST_CHANNEL  CAN_FIFO_CH0
#endif

//! CAN Filter Channels

typedef enum {
    CAN_FILTER0,
    CAN_FILTER1,
    CAN_FILTER2,
    CAN_FILTER3,
    CAN_FILTER4,
    CAN_FILTER5,
    CAN_FILTER6,
    CAN_FILTER7,
#ifdef CAN_FILTER_08TO15_IMPLEMENTED
    CAN_FILTER8,
    CAN_FILTER9,
    CAN_FILTER10,
    CAN_FILTER11,
    CAN_FILTER12,
    CAN_FILTER13,
    CAN_FILTER14,
    CAN_FILTER15,
#endif
#ifdef CAN_FILTER_16TO31_IMPLEMENTED
    CAN_FILTER16,
    CAN_FILTER17,
    CAN_FILTER18,
    CAN_FILTER19,
    CAN_FILTER20,
    CAN_FILTER21,
    CAN_FILTER22,
    CAN_FILTER23,
    CAN_FILTER24,
    CAN_FILTER25,
    CAN_FILTER26,
    CAN_FILTER27,
    CAN_FILTER28,
    CAN_FILTER29,
    CAN_FILTER30,
    CAN_FILTER31,
#endif
    CAN_FILTER_TOTAL
} CAN_FILTER;


//! CAN Operation Modes

typedef enum {
    CAN_NORMAL_MODE = 0x00,
    CAN_SLEEP_MODE = 0x01,
    CAN_INTERNAL_LOOPBACK_MODE = 0x02,
    CAN_LISTEN_ONLY_MODE = 0x03,
    CAN_CONFIGURATION_MODE = 0x04,
    CAN_EXTERNAL_LOOPBACK_MODE = 0x05,
    CAN_CLASSIC_MODE = 0x06,
    CAN_RESTRICTED_MODE = 0x07,
    CAN_INVALID_MODE = 0xFF
} CAN_OPERATION_MODE;

//! Transmit Bandwidth Sharing

typedef enum {
    CAN_TXBWS_NO_DELAY,
    CAN_TXBWS_2,
    CAN_TXBWS_4,
    CAN_TXBWS_8,
    CAN_TXBWS_16,
    CAN_TXBWS_32,
    CAN_TXBWS_64,
    CAN_TXBWS_128,
    CAN_TXBWS_256,
    CAN_TXBWS_512,
    CAN_TXBWS_1024,
    CAN_TXBWS_2048,
    CAN_TXBWS_4096
} CAN_TX_BANDWITH_SHARING;

//! Wake-up Filter Time

typedef enum {
    CAN_WFT00,
    CAN_WFT01,
    CAN_WFT10,
    CAN_WFT11
} CAN_WAKEUP_FILTER_TIME;

//! Data Byte Filter Number

typedef enum {
    CAN_DNET_FILTER_DISABLE = 0,
    CAN_DNET_FILTER_SIZE_1_BIT,
    CAN_DNET_FILTER_SIZE_2_BIT,
    CAN_DNET_FILTER_SIZE_3_BIT,
    CAN_DNET_FILTER_SIZE_4_BIT,
    CAN_DNET_FILTER_SIZE_5_BIT,
    CAN_DNET_FILTER_SIZE_6_BIT,
    CAN_DNET_FILTER_SIZE_7_BIT,
    CAN_DNET_FILTER_SIZE_8_BIT,
    CAN_DNET_FILTER_SIZE_9_BIT,
    CAN_DNET_FILTER_SIZE_10_BIT,
    CAN_DNET_FILTER_SIZE_11_BIT,
    CAN_DNET_FILTER_SIZE_12_BIT,
    CAN_DNET_FILTER_SIZE_13_BIT,
    CAN_DNET_FILTER_SIZE_14_BIT,
    CAN_DNET_FILTER_SIZE_15_BIT,
    CAN_DNET_FILTER_SIZE_16_BIT,
    CAN_DNET_FILTER_SIZE_17_BIT,
    CAN_DNET_FILTER_SIZE_18_BIT
} CAN_DNET_FILTER_SIZE;

//! FIFO Payload Size

typedef enum {
    CAN_PLSIZE_8,
    CAN_PLSIZE_12,
    CAN_PLSIZE_16,
    CAN_PLSIZE_20,
    CAN_PLSIZE_24,
    CAN_PLSIZE_32,
    CAN_PLSIZE_48,
    CAN_PLSIZE_64
} CAN_FIFO_PLSIZE;

//! CAN Configure

typedef struct _CAN_CONFIG {
    uint32_t DNetFilterCount : 5;
    uint32_t IsoCrcEnable : 1;
    uint32_t ProtocolExpectionEventDisable : 1;
    uint32_t WakeUpFilterEnable : 1;
    uint32_t WakeUpFilterTime : 2;
    uint32_t BitRateSwitchDisable : 1;
    uint32_t RestrictReTxAttempts : 1;
    uint32_t EsiInGatewayMode : 1;
    uint32_t SystemErrorToListenOnly : 1;
    uint32_t StoreInTEF : 1;
    uint32_t TXQEnable : 1;
    uint32_t TxBandWidthSharing : 4;
} CAN_CONFIG;

//! CAN Transmit Channel Configure

typedef struct _CAN_TX_FIFO_CONFIG {
    uint32_t RTREnable : 1;
    uint32_t TxPriority : 5;
    uint32_t TxAttempts : 2;
    uint32_t FifoSize : 5;
    uint32_t PayLoadSize : 3;
} CAN_TX_FIFO_CONFIG;

//! CAN Transmit Queue Configure

typedef struct _CAN_TX_QUEUE_CONFIG {
    uint32_t TxPriority : 5;
    uint32_t TxAttempts : 2;
    uint32_t FifoSize : 5;
    uint32_t PayLoadSize : 3;
} CAN_TX_QUEUE_CONFIG;

//! CAN Receive Channel Configure

typedef struct _CAN_RX_FIFO_CONFIG {
    uint32_t RxTimeStampEnable : 1;
    uint32_t FifoSize : 5;
    uint32_t PayLoadSize : 3;
} CAN_RX_FIFO_CONFIG;

//! CAN Transmit Event FIFO Configure

typedef struct _CAN_TEF_CONFIG {
    uint32_t TimeStampEnable : 1;
    uint32_t FifoSize : 5;
} CAN_TEF_CONFIG;

/* CAN Message Objects */

//! CAN Message Object ID

typedef struct _CAN_MSGOBJ_ID {
    uint32_t SID : 11;
    uint32_t EID : 18;
    uint32_t SID11 : 1;
    uint32_t unimplemented1 : 2;
} CAN_MSGOBJ_ID;

//! CAN Data Length Code
const uint8_t DLC_DataLength[16]={0,1,2,3,4,5,6,7,8,12,16,20,24,32,48,64};

//! CAN TX Message Object Control

typedef struct _CAN_TX_MSGOBJ_CTRL {
    uint32_t DLC : 4;
    uint32_t IDE : 1;
    uint32_t RTR : 1;
    uint32_t BRS : 1;
    uint32_t FDF : 1;
    uint32_t ESI : 1;
    uint32_t SEQ : 7;
    uint32_t unimplemented1 : 16;
} CAN_TX_MSGOBJ_CTRL;

//! CAN RX Message Object Control

typedef struct _CAN_RX_MSGOBJ_CTRL {
    uint32_t DLC : 4;
    uint32_t IDE : 1;
    uint32_t RTR : 1;
    uint32_t BRS : 1;
    uint32_t FDF : 1;
    uint32_t ESI : 1;
    uint32_t unimplemented1 : 2;
    uint32_t FilterHit : 5;
    uint32_t unimplemented2 : 16;
} CAN_RX_MSGOBJ_CTRL;

//! CAN Message Time Stamp
typedef uint32_t CAN_MSG_TIMESTAMP;

//! CAN TX Message Object

typedef union _CAN_TX_MSGOBJ {

    struct {
        CAN_MSGOBJ_ID id;
        CAN_TX_MSGOBJ_CTRL ctrl;
        CAN_MSG_TIMESTAMP timeStamp;
    } bF;
    uint32_t word[3];
    uint8_t byte[12];
} CAN_TX_MSGOBJ;

//! CAN RX Message Object

typedef union _CAN_RX_MSGOBJ {

    struct {
        CAN_MSGOBJ_ID id;
        CAN_RX_MSGOBJ_CTRL ctrl;
        CAN_MSG_TIMESTAMP timeStamp;
    } bF;
    uint32_t word[3];
    uint8_t byte[12];
} CAN_RX_MSGOBJ;

//! CAN TEF Message Object

typedef union _CAN_TEF_MSGOBJ {

    struct {
        CAN_MSGOBJ_ID id;
        CAN_TX_MSGOBJ_CTRL ctrl;
        CAN_MSG_TIMESTAMP timeStamp;
    } bF;
    uint32_t dword[3];
    uint8_t bytes[12];
} CAN_TEF_MSGOBJ;

//! CAN Filter Object ID

typedef struct _CAN_FILTEROBJ_ID {
    uint32_t SID : 11;
    uint32_t EID : 18;
    uint32_t SID11 : 1;
    uint32_t EXIDE : 1;
    uint32_t unimplemented1 : 1;
} CAN_FILTEROBJ_ID;

//! CAN Mask Object ID

typedef struct _CAN_MASKOBJ_ID {
    uint32_t MSID : 11;
    uint32_t MEID : 18;
    uint32_t MSID11 : 1;
    uint32_t MIDE : 1;
    uint32_t unimplemented1 : 1;
} CAN_MASKOBJ_ID;

//! CAN RX FIFO Status

typedef enum {
    CAN_RX_FIFO_EMPTY = 0,
    CAN_RX_FIFO_STATUS_MASK = 0x0F,
    CAN_RX_FIFO_NOT_EMPTY = 0x01,
    CAN_RX_FIFO_HALF_FULL = 0x02,
    CAN_RX_FIFO_FULL = 0x04,
    CAN_RX_FIFO_OVERFLOW = 0x08
} CAN_RX_FIFO_STATUS;

//! CAN TX FIFO Status

typedef enum {
    CAN_TX_FIFO_FULL = 0,
    CAN_TX_FIFO_STATUS_MASK = 0x1F7,
    CAN_TX_FIFO_NOT_FULL = 0x01,
    CAN_TX_FIFO_HALF_FULL = 0x02,
    CAN_TX_FIFO_EMPTY = 0x04,
    CAN_TX_FIFO_ATTEMPTS_EXHAUSTED = 0x10,
    CAN_TX_FIFO_ERROR = 0x20,
    CAN_TX_FIFO_ARBITRATION_LOST = 0x40,
    CAN_TX_FIFO_ABORTED = 0x80,
    CAN_TX_FIFO_TRANSMITTING = 0x100
} CAN_TX_FIFO_STATUS;

//! CAN TEF FIFO Status

typedef enum {
    CAN_TEF_FIFO_EMPTY = 0,
    CAN_TEF_FIFO_STATUS_MASK = 0x0F,
    CAN_TEF_FIFO_NOT_EMPTY = 0x01,
    CAN_TEF_FIFO_HALF_FULL = 0x02,
    CAN_TEF_FIFO_FULL = 0x04,
    CAN_TEF_FIFO_OVERFLOW = 0x08
} CAN_TEF_FIFO_STATUS;

//! CAN Module Event (Interrupts)

typedef enum {
    CAN_NO_EVENT = 0,
    CAN_ALL_EVENTS = 0xFF1F,
    CAN_TX_EVENT = 0x0001,
    CAN_RX_EVENT = 0x0002,
    CAN_TIME_BASE_COUNTER_EVENT = 0x0004,
    CAN_OPERATION_MODE_CHANGE_EVENT = 0x0008,
    CAN_TEF_EVENT = 0x0010,

    CAN_RAM_ECC_EVENT = 0x0100,
    CAN_SPI_CRC_EVENT = 0x0200,
    CAN_TX_ATTEMPTS_EVENT = 0x0400,
    CAN_RX_OVERFLOW_EVENT = 0x0800,
    CAN_SYSTEM_ERROR_EVENT = 0x1000,
    CAN_BUS_ERROR_EVENT = 0x2000,
    CAN_BUS_WAKEUP_EVENT = 0x4000,
    CAN_RX_INVALID_MESSAGE_EVENT = 0x8000
} CAN_MODULE_EVENT;

//! CAN TX FIFO Event (Interrupts)

typedef enum {
    CAN_TX_FIFO_NO_EVENT = 0,
    CAN_TX_FIFO_ALL_EVENTS = 0x17,
    CAN_TX_FIFO_NOT_FULL_EVENT = 0x01,
    CAN_TX_FIFO_HALF_FULL_EVENT = 0x02,
    CAN_TX_FIFO_EMPTY_EVENT = 0x04,
    CAN_TX_FIFO_ATTEMPTS_EXHAUSTED_EVENT = 0x10
} CAN_TX_FIFO_EVENT;

//! CAN RX FIFO Event (Interrupts)

typedef enum {
    CAN_RX_FIFO_NO_EVENT = 0,
    CAN_RX_FIFO_ALL_EVENTS = 0x0F,
    CAN_RX_FIFO_NOT_EMPTY_EVENT = 0x01,
    CAN_RX_FIFO_HALF_FULL_EVENT = 0x02,
    CAN_RX_FIFO_FULL_EVENT = 0x04,
    CAN_RX_FIFO_OVERFLOW_EVENT = 0x08,
	CAN_RX_FIFO_INVALID = 0xFF
} CAN_RX_FIFO_EVENT;

//! CAN TEF FIFO Event (Interrupts)

typedef enum {
    CAN_TEF_FIFO_NO_EVENT = 0,
    CAN_TEF_FIFO_ALL_EVENTS = 0x0F,
    CAN_TEF_FIFO_NOT_EMPTY_EVENT = 0x01,
    CAN_TEF_FIFO_HALF_FULL_EVENT = 0x02,
    CAN_TEF_FIFO_FULL_EVENT = 0x04,
    CAN_TEF_FIFO_OVERFLOW_EVENT = 0x08
} CAN_TEF_FIFO_EVENT;

//! CAN Bit Time Setup: Arbitration/Data Bit Phase

typedef enum {
    CAN_500K_1M,    // 0x00
    CAN_500K_2M,    // 0x01
    CAN_500K_3M,
    CAN_500K_4M,
    CAN_500K_5M,    // 0x04
    CAN_500K_6M7,
    CAN_500K_8M,    // 0x06
    CAN_500K_10M,
    CAN_250K_500K,  // 0x08
    CAN_250K_833K,
    CAN_250K_1M,
    CAN_250K_1M5,
    CAN_250K_2M,
    CAN_250K_3M,
    CAN_250K_4M,
    CAN_1000K_4M,   // 0x0f
    CAN_1000K_8M,
    CAN_125K_500K   // 0x11
} CAN_BITTIME_SETUP;

//! CAN Nominal Bit Time Setup

typedef enum {
    CAN_NBT_125K,
    CAN_NBT_250K,
    CAN_NBT_500K,
    CAN_NBT_1M
} CAN_NOMINAL_BITTIME_SETUP;

//! CAN Data Bit Time Setup

typedef enum {
    CAN_DBT_500K,
    CAN_DBT_833K,
    CAN_DBT_1M,
    CAN_DBT_1M5,
    CAN_DBT_2M,
    CAN_DBT_3M,
    CAN_DBT_4M,
    CAN_DBT_5M,
    CAN_DBT_6M7,
    CAN_DBT_8M,
    CAN_DBT_10M
} CAN_DATA_BITTIME_SETUP;

//! Secondary Sample Point Mode

typedef enum {
    CAN_SSP_MODE_OFF,
    CAN_SSP_MODE_MANUAL,
    CAN_SSP_MODE_AUTO
} CAN_SSP_MODE;

//! CAN Error State

typedef enum {
    CAN_ERROR_FREE_STATE = 0,
    CAN_ERROR_ALL = 0x3F,
    CAN_TX_RX_WARNING_STATE = 0x01,
    CAN_RX_WARNING_STATE = 0x02,
    CAN_TX_WARNING_STATE = 0x04,
    CAN_RX_BUS_PASSIVE_STATE = 0x08,
    CAN_TX_BUS_PASSIVE_STATE = 0x10,
    CAN_TX_BUS_OFF_STATE = 0x20
} CAN_ERROR_STATE;

//! CAN Time Stamp Mode Select

typedef enum {
    CAN_TS_SOF = 0x00,
    CAN_TS_EOF = 0x01,
    CAN_TS_RES = 0x02
} CAN_TS_MODE;

//! CAN ECC EVENT

typedef enum {
    CAN_ECC_NO_EVENT = 0x00,
    CAN_ECC_ALL_EVENTS = 0x06,
    CAN_ECC_SEC_EVENT = 0x02,
    CAN_ECC_DED_EVENT = 0x04
} CAN_ECC_EVENT;

//! CAN CRC EVENT

typedef enum {
    CAN_CRC_NO_EVENT = 0x00,
    CAN_CRC_ALL_EVENTS = 0x03,
    CAN_CRC_CRCERR_EVENT = 0x01,
    CAN_CRC_FORMERR_EVENT = 0x02
} CAN_CRC_EVENT;

//! GPIO Pin Position

typedef enum {
    GPIO_PIN_0,
    GPIO_PIN_1
} GPIO_PIN_POS;

//! GPIO Pin Modes

typedef enum {
    GPIO_MODE_INT,
    GPIO_MODE_GPIO
} GPIO_PIN_MODE;

//! GPIO Pin Directions

typedef enum {
    GPIO_OUTPUT,
    GPIO_INPUT
} GPIO_PIN_DIRECTION;

//! GPIO Open Drain Mode

typedef enum {
    GPIO_PUSH_PULL,
    GPIO_OPEN_DRAIN
} GPIO_OPEN_DRAIN_MODE;

//! GPIO Pin State

typedef enum {
    GPIO_LOW,
    GPIO_HIGH,
	INVALID
} GPIO_PIN_STATE;

//! Clock Output Mode

typedef enum {
    GPIO_CLKO_CLOCK,
    GPIO_CLKO_SOF
} GPIO_CLKO_MODE;

//! CAN Bus Diagnostic flags

typedef struct _CAN_BUS_DIAG_FLAGS {
    uint32_t NBIT0_ERR : 1;
    uint32_t NBIT1_ERR : 1;
    uint32_t NACK_ERR : 1;
    uint32_t NFORM_ERR : 1;
    uint32_t NSTUFF_ERR : 1;
    uint32_t NCRC_ERR : 1;
    uint32_t unimplemented1 : 1;
    uint32_t TXBO_ERR : 1;
    uint32_t DBIT0_ERR : 1;
    uint32_t DBIT1_ERR : 1;
    uint32_t unimplemented2 : 1;
    uint32_t DFORM_ERR : 1;
    uint32_t DSTUFF_ERR : 1;
    uint32_t DCRC_ERR : 1;
    uint32_t ESI : 1;
    uint32_t DLC_MISMATCH : 1;
} CAN_BUS_DIAG_FLAGS;

//! CAN Bus Diagnostic Error Counts

typedef struct _CAN_BUS_ERROR_COUNT {
    uint8_t NREC;
    uint8_t NTEC;
    uint8_t DREC;
    uint8_t DTEC;
} CAN_BUS_ERROR_COUNT;

//! CAN BUS DIAGNOSTICS

typedef union _CAN_BUS_DIAGNOSTIC {

    struct {
        CAN_BUS_ERROR_COUNT errorCount;
        uint16_t errorFreeMsgCount;
        CAN_BUS_DIAG_FLAGS flag;
    } bF;
    uint32_t dword[2];
    uint8_t bytes[8];
} CAN_BUS_DIAGNOSTIC;

//! TXREQ Channel Bits
// Multiple channels can be or'ed together

typedef enum {
    CAN_TXREQ_CH0 = 0x00000001,
    CAN_TXREQ_CH1 = 0x00000002,
    CAN_TXREQ_CH2 = 0x00000004,
    CAN_TXREQ_CH3 = 0x00000008,
    CAN_TXREQ_CH4 = 0x00000010,
    CAN_TXREQ_CH5 = 0x00000020,
    CAN_TXREQ_CH6 = 0x00000040,
    CAN_TXREQ_CH7 = 0x00000080,

    CAN_TXREQ_CH8 = 0x00000100,
    CAN_TXREQ_CH9 = 0x00000200,
    CAN_TXREQ_CH10 = 0x00000400,
    CAN_TXREQ_CH11 = 0x00000800,
    CAN_TXREQ_CH12 = 0x00001000,
    CAN_TXREQ_CH13 = 0x00002000,
    CAN_TXREQ_CH14 = 0x00004000,
    CAN_TXREQ_CH15 = 0x00008000,

    CAN_TXREQ_CH16 = 0x00010000,
    CAN_TXREQ_CH17 = 0x00020000,
    CAN_TXREQ_CH18 = 0x00040000,
    CAN_TXREQ_CH19 = 0x00080000,
    CAN_TXREQ_CH20 = 0x00100000,
    CAN_TXREQ_CH21 = 0x00200000,
    CAN_TXREQ_CH22 = 0x00400000,
    CAN_TXREQ_CH23 = 0x00800000,

    CAN_TXREQ_CH24 = 0x01000000,
    CAN_TXREQ_CH25 = 0x02000000,
    CAN_TXREQ_CH26 = 0x04000000,
    CAN_TXREQ_CH27 = 0x08000000,
    CAN_TXREQ_CH28 = 0x10000000,
    CAN_TXREQ_CH29 = 0x20000000,
    CAN_TXREQ_CH30 = 0x40000000,
    CAN_TXREQ_CH31 = 0x80000000
} CAN_TXREQ_CHANNEL;

//! Oscillator Control

typedef struct _CAN_OSC_CTRL {
    uint32_t PllEnable : 1;
    uint32_t OscDisable : 1;
    uint32_t SclkDivide : 1;
    uint32_t ClkOutDivide : 2;
} CAN_OSC_CTRL;

//! Oscillator Status

typedef struct _CAN_OSC_STATUS {
    uint32_t PllReady : 1;
    uint32_t OscReady : 1;
    uint32_t SclkReady : 1;
} CAN_OSC_STATUS;

//! ICODE

typedef enum {
    CAN_ICODE_FIFO_CH0,
    CAN_ICODE_FIFO_CH1,
    CAN_ICODE_FIFO_CH2,
    CAN_ICODE_FIFO_CH3,
    CAN_ICODE_FIFO_CH4,
    CAN_ICODE_FIFO_CH5,
    CAN_ICODE_FIFO_CH6,
    CAN_ICODE_FIFO_CH7,
#ifdef CAN_FIFO_08TO15_IMPLEMENTED
    CAN_ICODE_FIFO_CH8,
    CAN_ICODE_FIFO_CH9,
    CAN_ICODE_FIFO_CH10,
    CAN_ICODE_FIFO_CH11,
    CAN_ICODE_FIFO_CH12,
    CAN_ICODE_FIFO_CH13,
    CAN_ICODE_FIFO_CH14,
    CAN_ICODE_FIFO_CH15,
#endif
#ifdef CAN_FIFO_16TO31_IMPLEMENTED
    CAN_ICODE_FIFO_CH16,
    CAN_ICODE_FIFO_CH17,
    CAN_ICODE_FIFO_CH18,
    CAN_ICODE_FIFO_CH19,
    CAN_ICODE_FIFO_CH20,
    CAN_ICODE_FIFO_CH21,
    CAN_ICODE_FIFO_CH22,
    CAN_ICODE_FIFO_CH23,
    CAN_ICODE_FIFO_CH24,
    CAN_ICODE_FIFO_CH25,
    CAN_ICODE_FIFO_CH26,
    CAN_ICODE_FIFO_CH27,
    CAN_ICODE_FIFO_CH28,
    CAN_ICODE_FIFO_CH29,
    CAN_ICODE_FIFO_CH30,
    CAN_ICODE_FIFO_CH31,
#endif
    CAN_ICODE_TOTAL_CHANNELS,
    CAN_ICODE_NO_INT = 0x40,
    CAN_ICODE_CERRIF,
    CAN_ICODE_WAKIF,
    CAN_ICODE_RXOVIF,
    CAN_ICODE_ADDRERR_SERRIF,
    CAN_ICODE_MABOV_SERRIF,
    CAN_ICODE_TBCIF,
    CAN_ICODE_MODIF,
    CAN_ICODE_IVMIF,
    CAN_ICODE_TEFIF,
    CAN_ICODE_TXATIF,
    CAN_ICODE_RESERVED
} CAN_ICODE;

//! RXCODE

typedef enum {
    CAN_RXCODE_FIFO_CH0,
    CAN_RXCODE_FIFO_CH1,
    CAN_RXCODE_FIFO_CH2,
    CAN_RXCODE_FIFO_CH3,
    CAN_RXCODE_FIFO_CH4,
    CAN_RXCODE_FIFO_CH5,
    CAN_RXCODE_FIFO_CH6,
    CAN_RXCODE_FIFO_CH7,
#ifdef CAN_FIFO_08TO15_IMPLEMENTED
    CAN_RXCODE_FIFO_CH8,
    CAN_RXCODE_FIFO_CH9,
    CAN_RXCODE_FIFO_CH10,
    CAN_RXCODE_FIFO_CH11,
    CAN_RXCODE_FIFO_CH12,
    CAN_RXCODE_FIFO_CH13,
    CAN_RXCODE_FIFO_CH14,
    CAN_RXCODE_FIFO_CH15,
#endif
#ifdef CAN_FIFO_16TO31_IMPLEMENTED
    CAN_RXCODE_FIFO_CH16,
    CAN_RXCODE_FIFO_CH17,
    CAN_RXCODE_FIFO_CH18,
    CAN_RXCODE_FIFO_CH19,
    CAN_RXCODE_FIFO_CH20,
    CAN_RXCODE_FIFO_CH21,
    CAN_RXCODE_FIFO_CH22,
    CAN_RXCODE_FIFO_CH23,
    CAN_RXCODE_FIFO_CH24,
    CAN_RXCODE_FIFO_CH25,
    CAN_RXCODE_FIFO_CH26,
    CAN_RXCODE_FIFO_CH27,
    CAN_RXCODE_FIFO_CH28,
    CAN_RXCODE_FIFO_CH29,
    CAN_RXCODE_FIFO_CH30,
    CAN_RXCODE_FIFO_CH31,
#endif
    CAN_RXCODE_TOTAL_CHANNELS,
    CAN_RXCODE_NO_INT = 0x40,
    CAN_RXCODE_RESERVED
} CAN_RXCODE;

//! TXCODE

typedef enum {
    CAN_TXCODE_FIFO_CH0,
    CAN_TXCODE_FIFO_CH1,
    CAN_TXCODE_FIFO_CH2,
    CAN_TXCODE_FIFO_CH3,
    CAN_TXCODE_FIFO_CH4,
    CAN_TXCODE_FIFO_CH5,
    CAN_TXCODE_FIFO_CH6,
    CAN_TXCODE_FIFO_CH7,
#ifdef CAN_FIFO_08TO15_IMPLEMENTED
    CAN_TXCODE_FIFO_CH8,
    CAN_TXCODE_FIFO_CH9,
    CAN_TXCODE_FIFO_CH10,
    CAN_TXCODE_FIFO_CH11,
    CAN_TXCODE_FIFO_CH12,
    CAN_TXCODE_FIFO_CH13,
    CAN_TXCODE_FIFO_CH14,
    CAN_TXCODE_FIFO_CH15,
#endif
#ifdef CAN_FIFO_16TO31_IMPLEMENTED
    CAN_TXCODE_FIFO_CH16,
    CAN_TXCODE_FIFO_CH17,
    CAN_TXCODE_FIFO_CH18,
    CAN_TXCODE_FIFO_CH19,
    CAN_TXCODE_FIFO_CH20,
    CAN_TXCODE_FIFO_CH21,
    CAN_TXCODE_FIFO_CH22,
    CAN_TXCODE_FIFO_CH23,
    CAN_TXCODE_FIFO_CH24,
    CAN_TXCODE_FIFO_CH25,
    CAN_TXCODE_FIFO_CH26,
    CAN_TXCODE_FIFO_CH27,
    CAN_TXCODE_FIFO_CH28,
    CAN_TXCODE_FIFO_CH29,
    CAN_TXCODE_FIFO_CH30,
    CAN_TXCODE_FIFO_CH31,
#endif
    CAN_TXCODE_TOTAL_CHANNELS,
    CAN_TXCODE_NO_INT = 0x40,
    CAN_TXCODE_RESERVED
} CAN_TXCODE;

//! System Clock Selection

typedef enum {
    CAN_SYSCLK_40M,
    CAN_SYSCLK_20M,
    CAN_SYSCLK_10M
} CAN_SYSCLK_SPEED;

//! CLKO Divide

typedef enum {
    OSC_CLKO_DIV1,
    OSC_CLKO_DIV2,
    OSC_CLKO_DIV4,
    OSC_CLKO_DIV10
} OSC_CLKO_DIVIDE;

#endif // _DRV_CANFDSPI_DEFINES_H
