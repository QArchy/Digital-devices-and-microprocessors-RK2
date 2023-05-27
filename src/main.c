#include "main.h"

void main(void) {
    COMMAND command;
    INIT_RESET_COMMAND_STRUCTURE((&command))
    COMMAND_UPDATE command_update;
    INIT_RESET_COMMAND_UPDATE_STRUCTURE((&command_update))
    BUFFERS buffers;
    SETUP_PERIPHERALS(&buffers);

    while (1) {
        RECEIVE_COMMAND_VIA_USART
        PARSE_COMMAND(&command, buffers.rx_buffer);
        RESOLVE_COMMAND(&command, &command_update);
        EXECUTE_COMMAND(&command_update);
    }
}

void SETUP_GPIO(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // LEDS
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // For USART1 and ADC
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN; // For ADC
    // LEDS
    GPIOC->MODER |= GPIO_MODER_MODER8_0;
    GPIOC->MODER |= GPIO_MODER_MODER9_0;
}

void SETUP_PERIPHERALS(BUFFERS *buffers) {
    SETUP_GPIO();
    SETUP_DMA_USART1(buffers->tx_buffer, buffers->rx_buffer);
    SETUP_DMA_ADC(buffers->tx_buffer);
}
