/**
 * @file gpio_driver.h
 * @brief GPIO (General Purpose Input Output) driver for STM32F4xx microcontrollers.
 * @author Antigravity
 * @date 2026-07-27
 */

#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "stm32f4xx.h"

/**
 * @brief GPIO pin output types (OTYPER Register).
 */
typedef enum {
  GPIO_OP_TYPE_PP = 0x00U, /**< Push-Pull output configuration */
  GPIO_OP_TYPE_OD = 0x01U  /**< Open-Drain output configuration */
} GPIO_OpType_t;

/**
 * @brief GPIO pin speeds (OSPEEDR Register).
 */
typedef enum {
  GPIO_SPEED_LOW = 0x00U,    /**< Low speed configuration (up to 2 MHz) */
  GPIO_SPEED_MEDIUM = 0x01U, /**< Medium speed configuration (up to 25 MHz) */
  GPIO_SPEED_FAST = 0x02U,   /**< Fast speed configuration (up to 50 MHz) */
  GPIO_SPEED_HIGH = 0x03U    /**< High speed configuration (up to 100 MHz) */
} GPIO_Speed_t;

/**
 * @brief GPIO pin pull-up/pull-down configuration (PUPDR Register).
 */
typedef enum {
  GPIO_NO_PUPD = 0x00U, /**< No Pull-up or Pull-down resistor (Floating) */
  GPIO_PIN_PU = 0x01U,  /**< Pull-up resistor enabled */
  GPIO_PIN_PD = 0x02U   /**< Pull-down resistor enabled */
} GPIO_PuPd_t;

/**
 * @brief GPIO pin mode modes (MODER Register & EXTI).
 */
typedef enum {
  GPIO_MODE_IN = 0x00U,     /**< Input mode */
  GPIO_MODE_OUT = 0x01U,    /**< General purpose output mode */
  GPIO_MODE_ALTFN = 0x02U,  /**< Alternate function mode */
  GPIO_MODE_ANALOG = 0x03U, /**< Analog mode */
  GPIO_MODE_IT_FT = 0x04U,  /**< External Interrupt - Falling Edge Trigger */
  GPIO_MODE_IT_RT = 0x05U,  /**< External Interrupt - Rising Edge Trigger */
  GPIO_MODE_IT_RFT = 0x06U  /**< External Interrupt - Rising/Falling Edge Trigger */
} GPIO_Mode_t;

/* Pin Numaraları */
#define GPIO_PIN_0 0
#define GPIO_PIN_1 1
#define GPIO_PIN_2 2
#define GPIO_PIN_3 3
#define GPIO_PIN_4 4
#define GPIO_PIN_5 5   /**< Built-in LED on Nucleo boards (LD2 - Green LED) */
#define GPIO_PIN_6 6
#define GPIO_PIN_7 7
#define GPIO_PIN_8 8
#define GPIO_PIN_9 9
#define GPIO_PIN_10 10
#define GPIO_PIN_11 11
#define GPIO_PIN_12 12
#define GPIO_PIN_13 13 /**< Built-in Push Button (PC13) on Nucleo boards */
#define GPIO_PIN_14 14
#define GPIO_PIN_15 15

/**
 * @brief GPIO Pin Configuration Structure.
 */
typedef struct {
  uint8_t GPIO_PinNumber;       /**< Pin number from GPIO_PIN_0 to GPIO_PIN_15 */
  GPIO_Mode_t GPIO_PinMode;     /**< Pin operational mode (Input, Output, Alternate Function, Interrupt, etc.) */
  GPIO_Speed_t GPIO_PinSpeed;   /**< Pin output speed setting */
  GPIO_PuPd_t GPIO_PinPuPdControl; /**< Pin pull-up/pull-down resistor setting */
  GPIO_OpType_t GPIO_PinOPType;    /**< Pin output type setting */
  uint8_t GPIO_PinAltFunMode;   /**< Alternate Function Mode Number (AF0 - AF15) */
} GPIO_PinConfig_t;

/* Driver API Fonksiyonları */

/**
 * @brief Initializes the GPIO peripheral with given configurations.
 * @param pGPIOx Pointer to the base address of the GPIO peripheral (GPIOA, GPIOB, etc.)
 * @param pPinConfig Pointer to the GPIO pin configuration structure
 */
void GPIO_Init(GPIO_RegDef_t *pGPIOx, const GPIO_PinConfig_t *pPinConfig);

/**
 * @brief Writes a logic high or low value to a GPIO pin.
 * @param pGPIOx Pointer to the base address of the GPIO peripheral
 * @param pinNumber The pin number to write (0 to 15)
 * @param value The value to write (0 for low, any other value for high)
 */
void GPIO_WritePin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber, uint8_t value);

/**
 * @brief Toggles the state of a GPIO pin.
 * @param pGPIOx Pointer to the base address of the GPIO peripheral
 * @param pinNumber The pin number to toggle (0 to 15)
 */
void GPIO_TogglePin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber);

/**
 * @brief Reads the logic level of a GPIO pin.
 * @param pGPIOx Pointer to the base address of the GPIO peripheral
 * @param pinNumber The pin number to read (0 to 15)
 * @return uint8_t The state of the pin (0 or 1)
 */
uint8_t GPIO_ReadPin(const GPIO_RegDef_t *pGPIOx, uint8_t pinNumber);

/* NVIC ve IRQ Yardımcı API Fonksiyonları */

/**
 * @brief Configures the NVIC settings for the specified IRQ.
 * @param irqNumber The IRQ number (e.g. IRQ_NO_EXTI15_10)
 * @param priority The interrupt priority value
 * @param status Status value (1 to enable, 0 to disable)
 */
void GPIO_IRQConfig(uint8_t irqNumber, uint8_t priority, uint8_t status);

/**
 * @brief Handles EXTI interrupt flags. Should be called inside the ISR.
 * @param pinNumber The pin number that triggered the interrupt (0 to 15)
 */
void GPIO_IRQHandling(uint8_t pinNumber);

#endif /* GPIO_DRIVER_H */