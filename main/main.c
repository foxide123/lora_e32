#include "lora_e32.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "wifi_scan.h"

static const char* TAG = "MyModule";

void app_main(void)
{
    // Initializes NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    wifi_scan();

    LoRa_E32 e32;
    LoRa_E32_init(&e32, UART_NUM_1, GPIO_NUM_4, GPIO_NUM_7, GPIO_NUM_6, 9600);

    ESP_LOGI(TAG, "LoRa module initialized");

    /*while (1){
        send_broadcast_fixed_message(0x04, "Broadcast message to channel 04");
        ESP_LOGI(TAG, "Broadcast message sent");
        vTaskDelay(2000/portTICK_PERIOD_MS);
    }*/
}