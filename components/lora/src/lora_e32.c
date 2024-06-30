#include "lora_e32.h"
#include "esp_log.h"
#include <string.h>

void LoRa_E32_init(LoRa_E32* self, int uart_num, gpio_num_t auxPin, gpio_num_t rxdPin, gpio_num_t txdPin, int baud_rate) {
    self->uart_num = uart_num;
    self->auxPin = auxPin;
    self->rxdPin = rxdPin;
    self->txdPin = txdPin;
    self->baud_rate = baud_rate;

    uart_config_t uart_config ={
        .baud_rate = baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    uart_param_config(uart_num, &uart_config);
    uart_set_pin(uart_num, txdPin, rxdPin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(uart_num, UART_BUF_SIZE * 2, 0, 0, NULL, 0);

    // Set GPIO pins directions
    gpio_set_direction(auxPin, GPIO_MODE_INPUT);
}

void LoRa_E32_write(LoRa_E32* self, uint8_t* data, size_t length){
    uart_write_bytes(self->uart_num, (const char*)data, length);
}

void LoRa_E32_read(LoRa_E32* self, uint8_t* buffer, size_t length){
    uart_read_bytes(self->uart_num, buffer, length, pdMS_TO_TICKS(1000));
}

void send_broadcast_fixed_message(uint8_t chan, const char* message){
    char command[64];
    snprintf(command, sizeof(command), "AT+SEND=%02X, %s", chan, message);
    uart_write_bytes(UART_NUM_1, command, strlen(command));
}

Configuration getConfiguration() {
    Configuration configuration;

    //Initialized with dummy values for illustration
    configuration.HEAD = 0xC0;
    configuration.ADDH = 0x00;
    configuration.ADDL = 0x01;
    configuration.CHAN = 0x02;
    configuration.SPED.uartParity = 0x00;
    configuration.SPED.uartBaudRate = 0x03;
    configuration.SPED.airDataRate = 0x02;
    configuration.OPTION.fixedTransmission = 0x01;
    configuration.OPTION.ioDriveMode = 0x01;
    configuration.OPTION.wirelessWakeupTime = 0x02;
    configuration.OPTION.fec = 0x01;
    configuration.OPTION.transmissionPower = 0x02;

    return configuration;
}

void setConfiguration(Configuration* configuration){
    char command[256];
    snprintf(command, sizeof(command), "AT+CFG %02X %02X %02X %02X %02X %02X %02X %02X %02X",
             configuration->HEAD,
             configuration->ADDH,
             configuration->ADDL,
             configuration->SPED.uartParity,
             configuration->SPED.uartBaudRate,
             configuration->SPED.airDataRate,
             configuration->CHAN,
             configuration->OPTION.fixedTransmission,
             configuration->OPTION.ioDriveMode);

    //uart_write_bytes(UART_NUM, command, strlen(command));
}