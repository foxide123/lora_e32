#include "driver/uart.h"
#include "driver/gpio.h"

#define UART_BUF_SIZE (1024)

enum OPERATING_MODE
{
    MODE_0_NORMAL       = 0,
    MODE_1_WAKE_UP      = 1,
    MODE_2_POWER_SAVING = 2,
    MODE_3_SLEEP        = 3,
};

enum PROGRAM_COMMAND
{
  WRITE_CFG_PWR_DWN_SAVE  	= 0xC0,
  READ_CONFIGURATION 		= 0xC1,
  WRITE_CFG_PWR_DWN_LOSE 	= 0xC2,
  READ_MODULE_VERSION   	= 0xC3,
  WRITE_RESET_MODULE     	= 0xC4
};

typedef struct {
  uint8_t airDataRate;
  uint8_t uartBaudRate;
  uint8_t uartParity;
} Speed;

typedef struct {
  uint8_t transmissionPower;
  uint8_t fec;
  uint8_t wirelessWakeupTime;
  uint8_t ioDriveMode;
  uint8_t fixedTransmission;
} Option;

typedef struct {
  uint8_t HEAD;
  uint8_t ADDH;
  uint8_t ADDL;
  Speed SPED;
  uint8_t CHAN;
  Option OPTION;
} Configuration;

typedef struct {
    int uart_num;
    gpio_num_t auxPin;
    gpio_num_t rxdPin;
    gpio_num_t txdPin;
    int baud_rate;
} LoRa_E32;

void LoRa_E32_init(LoRa_E32* self, int uart_num, gpio_num_t auxPin, gpio_num_t m0Pin, gpio_num_t m1Pin, int baud_rate);
void LoRa_E32_write(LoRa_E32* self, uint8_t* data, size_t length);
void LoRa_E32_read(LoRa_E32* self, uint8_t* buffer, size_t length);
void send_broadcast_fixed_message(uint8_t chan, const char* message);
