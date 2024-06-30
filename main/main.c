#include "lora_e32.h"
#include "driver/uart.h"
#include "esp_log.h"

static const char* TAG = "MyModule";

void app_main(void)
{
    LoRa_E32 e32;
    LoRa_E32_init(&e32, UART_NUM_1, GPIO_NUM_4, GPIO_NUM_7, GPIO_NUM_6, 9600);

    ESP_LOGI(TAG, "LoRa module initialized");

    while (1){
        send_broadcast_fixed_message(0x04, "Broadcast message to channel 04");
        ESP_LOGI(TAG, "Broadcast message sent");
        vTaskDelay(2000/portTICK_PERIOD_MS);
    }
}