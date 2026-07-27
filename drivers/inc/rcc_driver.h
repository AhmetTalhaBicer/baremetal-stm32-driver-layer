// RCC (Reset and Clock Control) Sürücüsü
#ifndef RCC_DRIVER_H
#define RCC_DRIVER_H

#include "stm32f4xx.h"

/* GPIO Saat Etkinleştirme Makroları */
#define RCC_GPIOA_CLK_EN()                                                     \
  (RCC->AHB1ENR |=                                                             \
   (1U << 0)) // 1U << 0: 1 değerini 0. bite yerleştirir (GPIOA için)
#define RCC_GPIOB_CLK_EN() (RCC->AHB1ENR |= (1U << 1)) // SET 1
#define RCC_GPIOC_CLK_EN() (RCC->AHB1ENR |= (1U << 2))
#define RCC_GPIOD_CLK_EN() (RCC->AHB1ENR |= (1U << 3))

/* GPIO Saat Devre Dışı Bırakma Makroları */
#define RCC_GPIOA_CLK_DI() (RCC->AHB1ENR &= ~(1U << 0)) // SET 0
#define RCC_GPIOB_CLK_DI() (RCC->AHB1ENR &= ~(1U << 1))
#define RCC_GPIOC_CLK_DI() (RCC->AHB1ENR &= ~(1U << 2))
#define RCC_GPIOD_CLK_DI() (RCC->AHB1ENR &= ~(1U << 3))

#endif /* RCC_DRIVER_H */