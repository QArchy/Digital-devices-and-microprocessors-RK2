#include "dma_for_usart1.h"

uint8_t COMMAND_RECEIVED_VIA_USART = 0;
uint8_t ANSWER_TRANSMISSION_VIA_USART_ENDED = 0;

void DMA1_Channel2_3_IRQHandler(void) {
    if ((DMA1->ISR & DMA_ISR_TCIF2) == DMA_ISR_TCIF2) {
        DMA1->IFCR |= DMA_IFCR_CTCIF2;
        DMA1_Channel2->CCR &= ~DMA_CCR_EN; // turn off DMA transmission
        //size of data in byte
        DMA1_Channel2->CNDTR = MY_CNDTR_TX;
        ANSWER_TRANSMISSION_VIA_USART_ENDED = 1;
    }
    if ((DMA1->ISR & DMA_ISR_TCIF3) == DMA_ISR_TCIF3) {
        DMA1->IFCR |= DMA_IFCR_CTCIF3;
        DMA1_Channel3->CCR &= ~DMA_CCR_EN; // turn off DMA transmission
        //size of data in byte
        DMA1_Channel3->CNDTR = MY_CNDTR_RX;
        COMMAND_RECEIVED_VIA_USART = 1;
    }
}

static void setup_DMA_TX(const uint8_t *tx_buffer) {
    //memory increment mode
    DMA1_Channel2->CCR |= DMA_CCR_MINC;
    //dir of transfer data
    DMA1_Channel2->CCR |= DMA_CCR_DIR;
    //circular mode on
    //DMA1_Channel2->CCR |= DMA_CCR_CIRC;
    //size of data in byte
    DMA1_Channel2->CNDTR = MY_CNDTR_TX;
    //address of periphery
    DMA1_Channel2->CPAR = (uint32_t) (&(USART1->TDR));
    //address of data
    DMA1_Channel2->CMAR = (uint32_t) tx_buffer;
    //transmission complete interrupt
    DMA1_Channel2->CCR |= DMA_CCR_TCIE;
}

static void setup_DMA_RX(const uint8_t *rx_buffer) {
    //memory increment mode
    DMA1_Channel3->CCR |= DMA_CCR_MINC;
    //dir of transfer data
    DMA1_Channel3->CCR &= ~DMA_CCR_DIR;
    //circular mode on
    //DMA1_Channel3->CCR |= DMA_CCR_CIRC;
    //size of data in byte
    DMA1_Channel3->CNDTR = MY_CNDTR_RX;
    //address of periphery
    DMA1_Channel3->CPAR = (uint32_t) (&(USART1->RDR));
    //address of data
    DMA1_Channel3->CMAR = (uint32_t) rx_buffer;
    //transmission complete interrupt
    DMA1_Channel3->CCR |= DMA_CCR_TCIE;
}

static void setup_DMA_EN() {
    //enable interrupt
    NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1);
    NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
    //DMA on
    //DMA1_Channel2->CCR |= DMA_CCR_EN;
    //DMA1_Channel3->CCR |= DMA_CCR_EN;
}

void SETUP_DMA_FOR_USART1(const uint8_t *tx_buffer, const uint8_t *rx_buffer) {
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    setup_DMA_TX(tx_buffer);
    setup_DMA_RX(rx_buffer);
    setup_DMA_EN();
}