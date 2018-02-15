#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_sleep.h"
#include "sdkconfig.h"

#define TX_DATA_GPIO 22
#define TX_PWR_GPIO  23

#define BUTTON_A_GPIO 0
#define BUTTON_B_GPIO 2
#define BUTTON_A_BIT 0b0001
#define BUTTON_B_BIT 0b0100
#define BUTTON_BITS  0b0101

const short a1_on[100]  = {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
  1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,
  0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,
  0,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,0};
const short a1_off[100] = {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
  1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,
  0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,
  0,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0};
const short a2_on[100]  = {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
  1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,0,1,0,0,0,1,
  0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,
  0,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,0};
const short a2_off[100] = {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
  1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,0,1,0,0,0,1,
  0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,
  0,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0};
const short a3_on[100]  = {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
  1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,
  1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,
  0,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,0};
const short a3_off[100] = {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
  1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,
  1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,
  0,0,1,1,1,0,1,0,0,0,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0};

void transmit(const short bits[]) {
  int t, ix;
  for (t = 0; t < 3; t++) {
    for (ix = 0; ix < 100; ix++) {
      gpio_set_level(TX_DATA_GPIO, (bits[ix]) ? 1 : 0);
      ets_delay_us(320);
    }
    ets_delay_us(6000);
  }
  ets_delay_us(6000);
}

void turnOnTxPower() {
  gpio_set_level(TX_PWR_GPIO, 1);
  ets_delay_us(100000);
}

void turnOffTxPower() {
  gpio_set_level(TX_PWR_GPIO, 0);
}

void allLightsOn() {
  turnOnTxPower();
  transmit(a1_on);
  transmit(a2_on);
  transmit(a3_on);
  turnOffTxPower();
}

void allLightsOff() {
  turnOnTxPower();
  transmit(a1_off);
  transmit(a2_off);
  transmit(a3_off);
  turnOffTxPower();
}

void app_main() {
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT1)
  {
    gpio_pad_select_gpio(TX_PWR_GPIO);
    gpio_set_direction(TX_PWR_GPIO, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(TX_DATA_GPIO);
    gpio_set_direction(TX_DATA_GPIO, GPIO_MODE_OUTPUT);

    uint64_t map = esp_sleep_get_ext1_wakeup_status();
    if (map == BUTTON_B_BIT)
      allLightsOff();
    if (map == BUTTON_A_BIT)
      allLightsOn();
  }

  gpio_pullup_dis(BUTTON_A_GPIO);
  gpio_pulldown_en(BUTTON_A_GPIO);
  gpio_pullup_dis(BUTTON_B_GPIO);
  gpio_pulldown_en(BUTTON_B_GPIO);
  esp_sleep_enable_ext1_wakeup(
    BUTTON_BITS, ESP_EXT1_WAKEUP_ANY_HIGH);
  esp_sleep_pd_config(
    ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);

  esp_deep_sleep_start();
}
