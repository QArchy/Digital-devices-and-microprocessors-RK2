#ifndef SETUP_DMA_ADC_H_
#define SETUP_DMA_ADC_H_

#include "DMA_FOR_ADC/dma_for_adc.h"
#include "ADC/adc.h"
#include "stm32f051x8.h"

void SETUP_DMA_ADC(const uint8_t *buffer);

#endif // SETUP_DMA_ADC_H_