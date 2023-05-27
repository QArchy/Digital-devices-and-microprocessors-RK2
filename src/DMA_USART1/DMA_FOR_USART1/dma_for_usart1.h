#ifndef _USART1_DMA_DMA_H_
#define _USART1_DMA_DMA_H_

#include "stm32f051x8.h"

#define MY_CNDTR_RX 4
#define MY_CNDTR_TX 2

void SETUP_DMA_FOR_USART1(const uint8_t *tx_buffer, const uint8_t *rx_buffer);

#endif // _USART1_DMA_DMA_H_