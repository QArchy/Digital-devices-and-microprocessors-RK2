#ifndef MAIN_H_
#define MAIN_H_

#include "stm32f051x8.h"
#include "DMA_USART1/setup_DMA_USART1.h"
#include "DMA_ADC/setup_DMA_ADC.h"
#include "COMMAND_RESOLVE/command.h"

typedef struct BUFFERS {
    uint8_t tx_buffer[2]; // for ADC transmit
    uint8_t rx_buffer[4]; // for command receive
} BUFFERS;

void SETUP_GPIO(void);

void SETUP_PERIPHERALS(BUFFERS *buffers);

void main(void);

#endif /* MAIN_H_ */