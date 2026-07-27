#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "stm32f4xx.h"


/* GPIO Çıkış Tipleri (OTYPER Register) */
typedef enum {
  GPIO_OP_TYPE_PP = 0x00U, /* Push-Pull */
  GPIO_OP_TYPE_OD = 0x01U  /* Open-Drain */
} GPIO_OpType_t;

/* GPIO Hız Konfigürasyonu (OSPEEDR Register) */
typedef enum {
  GPIO_SPEED_LOW = 0x00U,
  GPIO_SPEED_MEDIUM = 0x01U,
  GPIO_SPEED_FAST = 0x02U,
  GPIO_SPEED_HIGH = 0x03U
} GPIO_Speed_t;

/* GPIO Pull-up / Pull-down Direnç Ayarı (PUPDR Register) */
typedef enum {
  GPIO_NO_PUPD = 0x00U, /* Direnç Yok (Floating) */
  GPIO_PIN_PU = 0x01U,  /* Dahili Pull-Up */
  GPIO_PIN_PD = 0x02U   /* Dahili Pull-Down */
} GPIO_PuPd_t;

typedef enum {
  GPIO_MODE_IN = 0x00U,
  GPIO_MODE_OUT = 0x01U,
  GPIO_MODE_ALTFN = 0x02U,
  GPIO_MODE_ANALOG = 0x03U,
  GPIO_MODE_IT_FT = 0x04U, /* External Interrupt - Falling Edge (Düşen Kenar) */
  GPIO_MODE_IT_RT =
      0x05U, /* External Interrupt - Rising Edge (Yükselen Kenar) */
  GPIO_MODE_IT_RFT = 0x06U /* External Interrupt - Rising/Falling Edge */
} GPIO_Mode_t;

/* Pin Numaraları */
#define GPIO_PIN_0 0
#define GPIO_PIN_1 1
#define GPIO_PIN_2 2
#define GPIO_PIN_3 3
#define GPIO_PIN_4 4
#define GPIO_PIN_5                                                             \
  5 /* Genellikle Nucleo kartlarda LD2 Green LED bu pindedir (PA5) */
#define GPIO_PIN_6 6
#define GPIO_PIN_7 7
#define GPIO_PIN_8 8
#define GPIO_PIN_9 9
#define GPIO_PIN_10 10
#define GPIO_PIN_11 11
#define GPIO_PIN_12 12
#define GPIO_PIN_13 13 /* Genellikle Push Button (PC13) */
#define GPIO_PIN_14 14
#define GPIO_PIN_15 15

/* GPIO Konfigürasyon Yapısı */
typedef struct {
  uint8_t GPIO_PinNumber; /* Pin 0 - 15 */
  GPIO_Mode_t GPIO_PinMode;
  GPIO_Speed_t GPIO_PinSpeed;
  GPIO_PuPd_t GPIO_PinPuPdControl;
  GPIO_OpType_t GPIO_PinOPType;
  uint8_t GPIO_PinAltFunMode; /* Alternatif Fonksiyon Numarası (AF0 - AF15) */
} GPIO_PinConfig_t;

/* Driver API Fonksiyonları */
void GPIO_Init(GPIO_RegDef_t *pGPIOx, const GPIO_PinConfig_t *pPinConfig);
void GPIO_WritePin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber, uint8_t value);
void GPIO_TogglePin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber);
uint8_t GPIO_ReadPin(const GPIO_RegDef_t *pGPIOx, uint8_t pinNumber);

/* NVIC ve IRQ Yardımcı API Fonksiyonları */
void GPIO_IRQConfig(uint8_t irqNumber, uint8_t priority, uint8_t status);
void GPIO_IRQHandling(uint8_t pinNumber);

#endif /* GPIO_DRIVER_H */