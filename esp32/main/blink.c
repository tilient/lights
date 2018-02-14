#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_sleep.h"
#include "sdkconfig.h"

#define TX_DATA_GPIO 22
#define TX_PWR_GPIO 23
#define BUTTON_A_GPIO 0

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

void send(const short bits[]) {
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

void allOn() {
  gpio_set_level(TX_PWR_GPIO, 1);
  ets_delay_us(100000);
  send(a1_on);
  send(a2_on);
  send(a3_on);
  gpio_set_level(TX_PWR_GPIO, 0);
}

void allOff() {
  gpio_set_level(TX_PWR_GPIO, 1);
  ets_delay_us(100000);
  send(a1_off);
  send(a2_off);
  send(a3_off);
  gpio_set_level(TX_PWR_GPIO, 0);
}


void app_main() {
  gpio_pad_select_gpio(TX_DATA_GPIO);
  gpio_set_direction(TX_DATA_GPIO, GPIO_MODE_OUTPUT);

  gpio_pad_select_gpio(TX_PWR_GPIO);
  gpio_set_direction(TX_PWR_GPIO, GPIO_MODE_OUTPUT);

  //vTaskDelay(10000 / portTICK_PERIOD_MS);
  allOn();

  //esp_sleep_enable_timer_wakeup(725000);

  gpio_pullup_en(BUTTON_A_GPIO);
  gpio_pulldown_dis(BUTTON_A_GPIO);
  esp_sleep_enable_ext0_wakeup(BUTTON_A_GPIO, 0);

  esp_deep_sleep_start();
}
