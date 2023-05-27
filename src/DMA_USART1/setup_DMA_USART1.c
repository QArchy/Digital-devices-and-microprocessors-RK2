#include "setup_DMA_USART1.h"

void SETUP_DMA_USART1(const uint8_t *tx_buffer, const uint8_t *rx_buffer) {
    SETUP_USART1();
    SETUP_DMA_FOR_USART1(tx_buffer, rx_buffer);
}