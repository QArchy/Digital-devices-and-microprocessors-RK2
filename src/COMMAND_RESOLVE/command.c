#include "command.h"

extern uint8_t COMMAND_RECEIVED_VIA_USART;
extern uint8_t ADC_TRANSMISSION_ENDED;
extern uint8_t ANSWER_SEND_VIA_USART;

#define END_OF_ADC_TRANSMISSION(predicate) (predicate && ADC_TRANSMISSION_ENDED)

void PARSE_COMMAND(COMMAND *command, const uint8_t *buffer) {
    if (!COMMAND_RECEIVED_VIA_USART) { return; }
    COMMAND_RECEIVED_VIA_USART = 0;
    command->sync = buffer[0];
    if (command->sync != 0xAB) {
        command->sync = 0xAB;
        return;
    }
    command->command_number = buffer[1];
    command->command_data.command_data_1 = buffer[2]; /* 15-8 bit */
    command->command_data.command_data_0 = buffer[3]; /* 7-0 bit */
    command->xor_key = command->sync ^ command->command_number;
}

void RESOLVE_COMMAND(COMMAND *command, COMMAND_UPDATE *command_update) {
    switch (command->command_number) {
        case SET_TRGO_FREQUENCY: { /* 0x01 */
            command_update->update_trgo_frequency.trgo_frequency = ((uint16_t) command->command_data.command_data_0) \
 | (((uint16_t) command->command_data.command_data_1) << 8);
            command_update->update_trgo_frequency.update_flag = 1;
            GPIOC->ODR ^= GPIO_ODR_8;
            break;
        }
        case SWITCH_ADC: /* 0x02 */
            command_update->update_switch_adc.switch_adc ^= 1;
            command_update->update_switch_adc.update_flag = 1;
            GPIOC->ODR ^= GPIO_ODR_8;
            break;
        case GET_ADC_DATA: /* 0x03 */
            command_update->update_get_adc_data.get_adc_data = ((uint16_t) command->command_data.command_data_0) \
 | (((uint16_t) command->command_data.command_data_1) << 8);
            command_update->update_get_adc_data.update_flag = 1;
            GPIOC->ODR ^= GPIO_ODR_8;
            break;
        case SWITCH_GET_ADC_DATA_INFINITELY: /* 0x04 */
            command_update->update_switch_get_adc_data_infinitely.switch_get_adc_data_infinitely ^= 1;
            command_update->update_switch_get_adc_data_infinitely.update_flag = 1;
            GPIOC->ODR ^= GPIO_ODR_8;
            break;
        default:
            break;
    }
    INIT_RESET_COMMAND_STRUCTURE(command)
}

static uint32_t get_adc_frequency(uint16_t data) {
    uint8_t ones_quantity = 0;
    for (uint8_t i = 0; i < 16; i++) {
        if (data & (1 << i)) {
            ones_quantity++;
        }
    }

    return ((uint32_t) 1) << ones_quantity;
}

void EXECUTE_COMMAND(COMMAND_UPDATE *command_update) {
    if (command_update->update_trgo_frequency.update_flag) { /* 0x01 */
        command_update->update_trgo_frequency.update_flag = 0;
        TIM15->CR1 &= ~TIM_CR1_CEN;
        TIM15->ARR = command_update->update_trgo_frequency.trgo_frequency == 0 ? 2 :
                     get_adc_frequency(command_update->update_trgo_frequency.trgo_frequency);
        TIM15->CR1 |= TIM_CR1_CEN;
        GPIOC->ODR ^= GPIO_ODR_9;
    }
    if (command_update->update_switch_adc.update_flag) { /* 0x02 */
        command_update->update_switch_adc.update_flag = 0;
        if (command_update->update_switch_adc.switch_adc) {
            ADC1->CR |= ADC_CR_ADSTP;
        } else {
            ADC1->CR |= ADC_CR_ADSTART;
        }
        GPIOC->ODR ^= GPIO_ODR_9;
    }
    if (command_update->update_get_adc_data.update_flag) { /* 0x03 */
        command_update->update_get_adc_data.update_flag = 0;
        GPIOC->ODR ^= GPIO_ODR_9;
    }
    if (command_update->update_switch_get_adc_data_infinitely.update_flag) { /* 0x04 */
        command_update->update_switch_get_adc_data_infinitely.update_flag = 0;
        GPIOC->ODR ^= GPIO_ODR_9;
    }
    /// Loop command actions
    // infinite print
    if (END_OF_ADC_TRANSMISSION(command_update->update_switch_get_adc_data_infinitely.switch_get_adc_data_infinitely)) {
        ADC_TRANSMISSION_ENDED = 0;
        SEND_ANSWER_VIA_USART
    }
    // exact print
    if (END_OF_ADC_TRANSMISSION(command_update->update_get_adc_data.get_adc_data)) {
        ADC_TRANSMISSION_ENDED = 0;
        if (ANSWER_SEND_VIA_USART) {
            ANSWER_SEND_VIA_USART = 0;
            command_update->update_get_adc_data.get_adc_data--;
        }
        SEND_ANSWER_VIA_USART
    }
}