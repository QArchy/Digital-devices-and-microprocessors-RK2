#ifndef DZ2_USART_DMA_SETUP
#define DZ2_USART_DMA_SETUP

#include "DMA_FOR_USART1/dma_for_usart1.h"
#include "USART1/usart1.h"

void SETUP_DMA_USART1(const uint8_t *tx_buffer, const uint8_t *rx_buffer);

#endif