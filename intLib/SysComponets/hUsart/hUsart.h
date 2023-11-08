#ifndef __hUsart_H__
#define __hUsart_H__
#include "at32f435_437_usart.h"
#include "at32f435_437_edma.h"
#include <stdio.h>
#include <stdlib.h>
extern uint32_t UartVirtStateRegister;

// определяет способ выделения памяти 1=статическая память; !1 - динамически
#define STATIC_MEM 1 

#if STATIC_MEM == 1
#define DESCRIPTORS_SIZE_BUF 5
#else

#endif

#define EDMA_NVIC_PRIOR 3
#define EDMA_NVIC_SUB_PRIOR 2
#define EDMA_COUNT_STREAM 8

// для установки битов в статусном виртуальном регистре
enum UartVirRegBits
{
    USART1_EN_CLOCK,
    USART2_EN_CLOCK,
    USART3_EN_CLOCK,
    UART4_EN_CLOCK,
    UART5_EN_CLOCK,
    USART6_EN_CLOCK,
    UART7_EN_CLOCK,
    UART_FREE_BIT,
    EDMA_STREAM1_EN,
    EDMA_STREAM2_EN,
    EDMA_STREAM3_EN,
    EDMA_STREAM4_EN,
    EDMA_STREAM5_EN,
    EDMA_STREAM6_EN,
    EDMA_STREAM7_EN,
    EDMA_STREAM8_EN,

    EDMA_ERR = 0xe0000000,
    USART_ERR = 0x0e000000,
};

typedef struct hUsartParam
{
    uint32_t baud_rate;
    usart_data_bit_num_type data_bit;
    usart_stop_bit_num_type stop_bit;
} hUsartParam_t;

typedef struct hUsartHand
{
    usart_type *usart_x;
    hUsartParam_t param;
} hUsartHand_t;

typedef struct hEdmaDescriptorData
{
    uint8_t *ptrBuf;
    uint32_t bufSize;
    usart_type *usart_x;
} hEdmaDescriptorData_t;

typedef struct hEdmaDescriptors
{
    uint32_t ctrl_dtcnt;
    uint32_t paddr;
    uint32_t m0addr;
    uint32_t nextLLp;
} hEdmaDescriptors_t;

typedef struct hEdmaUsartHand
{
    edma_stream_type *edma_streamx;
    edmamux_channel_type *edmamux_channelx;
    usart_type *usart_x;
} hEdmaUsartHand_t;

int hUsartInit(hUsartHand_t *hand_x);
int hUsartEdmaInit(hEdmaUsartHand_t *hand_x);
int hUsartEdmaAddDescriptors(hEdmaDescriptorData_t *EdmaDesData, edma_stream_type *edma_streamx);
void hUsartTransmitByte(usart_type *usart_x, uint8_t byte);
void hUsartTransmitData(usart_type *usart_x, uint8_t *data, uint32_t DataSize);
void hUsartEdmaStartTransmit(hEdmaUsartHand_t *hand_x);
void PrintList(edma_stream_type *edma_streamx);
#endif //__hUsart_H__