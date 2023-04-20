#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define CONFIG_FREERTOS_HZ 100

// Define o canal do ADC a ser lido
#define ADC_CHANNEL    ADC_CHANNEL_6

void app_main(void)
{
    // Configuração do ADC
    esp_err_t err;
    adc1_config_width(ADC_WIDTH_BIT_10);                                 // Define a largura dos dados do ADC
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_11);            // Define o canal e a atenuação do ADC

    // Caracterização do ADC
    esp_adc_cal_characteristics_t *adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    err = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_10, 1100, adc_chars);
    if (err != ESP_OK) {
        printf("Erro ao caracterizar o ADC! Código de erro: %d\n", err);
        return;
    }

    // Loop principal
    while(1) {
        
        uint32_t adc_reading = adc1_get_raw(ADC_CHANNEL);                               // Lê o valor do ADC

        
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);          // Converte o valor lido para tensão

        // Imprime o valor lido
        printf("Valor lido: %ld\n", adc_reading);
        printf("Tensão: %ldmV\n", voltage);

       
        vTaskDelay(pdMS_TO_TICKS(1000));                                                 // Aguarda 1 segundo
    }

    free(adc_chars);
}
