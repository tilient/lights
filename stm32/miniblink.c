#include <libopencm3/stm32/rcc.h>
#include <libopencmsis/core_cm3.h>
#include <libopencm3/cm3/dwt.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/f1/pwr.h>
#include <libopencm3/stm32/exti.h>

// ---------------------------------------------------

static void setup(void)
{
  // enable port C12 as outpur - on board led
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(
    GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

  // enable port A0 as input
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_set_mode(
    GPIOA, GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_FLOAT, GPIO0);

  // enable cycle counter
  dwt_enable_cycle_counter();

  // setup NVIC
  nvic_enable_irq(NVIC_EXTI0_IRQ);

  // setup EXTI
  exti_select_source(EXTI0, GPIOA);
  exti_set_trigger(EXTI0, EXTI_TRIGGER_RISING);
  exti_enable_request(EXTI0);
}

// ---------------------------------------------------

static void delay_us(uint32_t micros) {
  uint32_t t0 = dwt_read_cycle_counter();
  uint32_t delta = (72 * micros) / 10;
  while ((dwt_read_cycle_counter() - t0) < delta);
}

static void delay_ms(uint32_t millis) {
  delay_us(1000 * millis);
}

// ---------------------------------------------------

uint8_t t = 1;

void exti0_isr() {
  exti_reset_request(EXTI0);
  if (t)
    t = 0;
  else
    t = 1;
}

// ---------------------------------------------------

int main(void)
{
	setup();
	while (1) {
	  gpio_toggle(GPIOC, GPIO13);
    if (t)
      delay_ms(100);
    else
      delay_ms(500);
	}
	return 0;
}
