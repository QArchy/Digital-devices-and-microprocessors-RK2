#include "adc.h"

static void SETUP_GPIO_FOR_ADC(void) {
    GPIOA->MODER |= GPIO_MODER_MODER0;	//pa0 as analog function
    GPIOB->MODER |= GPIO_MODER_MODER0;	//pb0 as analog function
}

static void SETUP_DMA_FOR_ADC(void) {
    ADC1->CFGR1 |= ADC_CFGR1_DMACFG;
    ADC1->CFGR1 |= ADC_CFGR1_DMAEN;
}

static void SETUP_TIM15_FOR_ADC() {
    RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
    TIM15->ARR = 2;
    TIM15->PSC = 999;
    TIM15->CR2 |= TIM_CR2_MMS_1; //update tim15_trgO
    TIM15->CR1 |= TIM_CR1_CEN;
}

static void CONFIGURE_TIM15_AS_TRGO() {
    ADC1->CFGR1 &= ~ADC_CFGR1_CONT;
    ADC1->CFGR1 |= ADC_CFGR1_DISCEN;
    ADC1->CFGR1 |= ADC_CFGR1_EXTEN_0;
    ADC1->CFGR1 |= ADC_CFGR1_EXTSEL_2;
}

static void START_ADC() {
    ADC1->CR |= ADC_CR_ADEN;
    while((ADC1->ISR & ADC_ISR_ADRDY) != ADC_ISR_ADRDY);
    ADC1->CR |= ADC_CR_ADSTART;
}

void SETUP_ADC(void) {
    SETUP_GPIO_FOR_ADC();

    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->CR2 |= RCC_CR2_HSI14ON;

    ADC1->CFGR1 |= ADC_CFGR1_CONT;
    ADC1->CFGR1 |= ADC_CFGR1_OVRMOD;
    ADC1->SMPR &= ~ADC_SMPR1_SMPR;	//0x07 239.5 adc cycles
    ADC1->CHSELR = 0x0001;	//1 and 8 channels are ON...

    SETUP_DMA_FOR_ADC();
    SETUP_TIM15_FOR_ADC();
    CONFIGURE_TIM15_AS_TRGO();
    START_ADC();
}
