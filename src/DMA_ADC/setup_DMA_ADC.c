#include "setup_DMA_ADC.h"

void SETUP_DMA_ADC(const uint8_t *buffer) {
    SETUP_ADC();
    SETUP_DMA_FOR_ADC(buffer);
}
