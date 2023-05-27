#include "stm32f051x8.h"

int main(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER |= GPIO_MODER_MODER8_0;
    while (1) {
        GPIOC->ODR ^= GPIO_ODR_8;
        for (uint32_t i = 0; i < 100000; i++);
    }
}
