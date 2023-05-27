#ifndef COMMAND_H_
#define COMMAND_H_

#include "stm32f051x8.h"

typedef struct COMMAND {
    uint8_t sync;
    uint8_t command_number;
    struct {
        uint8_t command_data_1; /* 15-8 bit */
        uint8_t command_data_0; /* 7-0 bit */
    } command_data;
    uint8_t xor_key;
} COMMAND;

enum COMMAND_NAME {
    SET_TRGO_FREQUENCY = 0x01,
    SWITCH_ADC = 0x02,
    GET_ADC_DATA = 0x03,
    SWITCH_GET_ADC_DATA_INFINITELY = 0x04,
};

typedef struct COMMAND_UPDATE {
    struct {
        uint16_t trgo_frequency;
        uint8_t update_flag;
    } update_trgo_frequency;
    struct {
        uint8_t switch_adc;
        uint8_t update_flag;
    } update_switch_adc;
    struct {
        uint16_t get_adc_data;
        uint8_t update_flag;
    } update_get_adc_data;
    struct {
        uint8_t switch_get_adc_data_infinitely;
        uint8_t update_flag;
    } update_switch_get_adc_data_infinitely;
} COMMAND_UPDATE;

#define RECEIVE_COMMAND_VIA_USART if (!(DMA1_Channel3->CCR & DMA_CCR_EN)) { DMA1_Channel3->CCR |= DMA_CCR_EN; }
#define SEND_ANSWER_VIA_USART if (!(DMA1_Channel2->CCR & DMA_CCR_EN)) { DMA1_Channel2->CCR |= DMA_CCR_EN; }
#define INIT_RESET_COMMAND_STRUCTURE(command) command->sync = 0xAB; command->command_number = 0x00; \
command->command_data.command_data_0 = 0x00; command->command_data.command_data_1 = 0x00; command->xor_key = 0x00;
#define INIT_RESET_COMMAND_UPDATE_STRUCTURE(command_update) \
    command_update->update_trgo_frequency.trgo_frequency = 0; \
    command_update->update_trgo_frequency.update_flag = 0; \
    command_update->update_switch_adc.switch_adc = 0; \
    command_update->update_switch_adc.update_flag = 0; \
    command_update->update_get_adc_data.get_adc_data = 0; \
    command_update->update_get_adc_data.update_flag = 0; \
    command_update->update_switch_get_adc_data_infinitely.switch_get_adc_data_infinitely = 0; \
    command_update->update_switch_get_adc_data_infinitely.update_flag = 0;

void PARSE_COMMAND(COMMAND *command, const uint8_t *buffer);

void RESOLVE_COMMAND(COMMAND *command, COMMAND_UPDATE *command_update);

void EXECUTE_COMMAND(COMMAND_UPDATE *command_update);

#endif /* COMMAND_H_ */