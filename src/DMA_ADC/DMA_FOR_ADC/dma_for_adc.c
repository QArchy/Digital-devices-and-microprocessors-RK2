#include "dma_for_adc.h"

uint8_t ADC_TRANSMISSION_ENDED = 0;

void DMA1_Channel1_IRQHandler(void) {
    DMA1->IFCR |= DMA_IFCR_CTCIF1;
    ADC_TRANSMISSION_ENDED = 1;
}

void SETUP_DMA_FOR_ADC(const uint8_t *buffer) {
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    //memory increment mode
    DMA1_Channel1->CCR |= DMA_CCR_MINC;
    //dir of transfer data
    DMA1_Channel1->CCR &= ~DMA_CCR_DIR;
    //circular mode on
    DMA1_Channel1->CCR |= DMA_CCR_CIRC;
    //size of data in byte
    DMA1_Channel1->CNDTR = 2;
    //address of periphery
    DMA1_Channel1->CPAR = (uint32_t) (&(ADC1->DR));
    //address of data
    DMA1_Channel1->CMAR = (uint32_t) buffer;
    //transmission complete interrupt
    DMA1_Channel1->CCR |= DMA_CCR_TCIE;
    // enable interrupt
    NVIC_SetPriority(DMA1_Ch1_IRQn, 0);
    NVIC_EnableIRQ(DMA1_Ch1_IRQn);

    DMA1_Channel1->CCR |= DMA_CCR_EN;
}
