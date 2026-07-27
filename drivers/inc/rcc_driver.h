/**
 * @file rcc_driver.h
 * @brief RCC (Reset and Clock Control) driver macros for enabling peripheral clocks.
 * @author Antigravity
 * @date 2026-07-27
 */

#ifndef RCC_DRIVER_H
#define RCC_DRIVER_H

#include "stm32f4xx.h"

/* GPIO Saat Etkinleştirme Makroları */

/** @brief Enable clock for GPIO Port A */
#define RCC_GPIOA_CLK_EN() (RCC->AHB1ENR |= (1U << 0))

/** @brief Enable clock for GPIO Port B */
#define RCC_GPIOB_CLK_EN() (RCC->AHB1ENR |= (1U << 1))

/** @brief Enable clock for GPIO Port C */
#define RCC_GPIOC_CLK_EN() (RCC->AHB1ENR |= (1U << 2))

/** @brief Enable clock for GPIO Port D */
#define RCC_GPIOD_CLK_EN() (RCC->AHB1ENR |= (1U << 3))

/* GPIO Saat Devre Dışı Bırakma Makroları */

/** @brief Disable clock for GPIO Port A */
#define RCC_GPIOA_CLK_DI() (RCC->AHB1ENR &= ~(1U << 0))

/** @brief Disable clock for GPIO Port B */
#define RCC_GPIOB_CLK_DI() (RCC->AHB1ENR &= ~(1U << 1))

/** @brief Disable clock for GPIO Port C */
#define RCC_GPIOC_CLK_DI() (RCC->AHB1ENR &= ~(1U << 2))

/** @brief Disable clock for GPIO Port D */
#define RCC_GPIOD_CLK_DI() (RCC->AHB1ENR &= ~(1U << 3))

#endif /* RCC_DRIVER_H */