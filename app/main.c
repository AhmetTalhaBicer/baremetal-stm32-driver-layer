#include "../drivers/inc/gpio_driver.h"
#include "../drivers/inc/rcc_driver.h"
#include "../drivers/inc/stm32f4xx.h"

/* Basit Gecikme Fonksiyonu */
static void delay(volatile uint32_t count) {
  while (count--) {
    __asm("nop");
  }
}

int main(void) {
  /* 1. GPIOA Saat Sinyalini Aç */
  RCC_GPIOA_CLK_EN();

  /* 2. PA5 Pini (LED) Ayarları */
  GPIO_PinConfig_t ledPinConfig = {.GPIO_PinNumber = GPIO_PIN_5,
                                   .GPIO_PinMode = GPIO_MODE_OUT,
                                   .GPIO_PinSpeed = GPIO_SPEED_LOW,
                                   .GPIO_PinOPType = GPIO_OP_TYPE_PP,
                                   .GPIO_PinPuPdControl = GPIO_NO_PUPD};

  /* 3. GPIO Pinini İlklendir */
  GPIO_Init(GPIOA, &ledPinConfig);

  /* 4. Ana Döngü (Blink LED) */
  while (1) {
    GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    delay(500000);
  }

  return 0;
}