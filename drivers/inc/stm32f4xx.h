#ifndef STM32F4XX_H
#define STM32F4XX_H

#include <stddef.h>
#include <stdint.h>

/* ========================================================================== */
/* 1. BELLEK TABAN (BASE) ADRESLERİ                                          */
/* ========================================================================== */

#define FLASH_BASEADDR 0x08000000UL  /* Flash Bellek Taban Adresi */
#define SRAM1_BASEADDR 0x20000000UL  /* SRAM1 Taban Adresi (112 KB) */
#define PERIPH_BASEADDR 0x40000000UL /* Çevre Birimleri Taban Adresi */

/* Veri Yolu (Bus) Taban Adresleri */
#define APB1PERIPH_BASEADDR PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR (PERIPH_BASEADDR + 0x00010000UL)
#define AHB1PERIPH_BASEADDR (PERIPH_BASEADDR + 0x00020000UL)
#define AHB2PERIPH_BASEADDR 0x50000000UL

/* AHB1 Veri Yolu Üzerindeki Çevre Birimleri Adresleri */
#define GPIOA_BASEADDR (AHB1PERIPH_BASEADDR + 0x0000UL)
#define GPIOB_BASEADDR (AHB1PERIPH_BASEADDR + 0x0400UL)
#define GPIOC_BASEADDR (AHB1PERIPH_BASEADDR + 0x0800UL)
#define GPIOD_BASEADDR (AHB1PERIPH_BASEADDR + 0x0C00UL)
#define RCC_BASEADDR (AHB1PERIPH_BASEADDR + 0x3800UL)

/* SYSCFG ve EXTI Taban Adresleri */
#define SYSCFG_BASEADDR (APB2PERIPH_BASEADDR + 0x3800UL)
#define EXTI_BASEADDR (APB2PERIPH_BASEADDR + 0x3C00UL)

/* Cortex-M4 NVIC Register Taban Adresleri (Internal Core Peripherals) */
#define NVIC_ISER_BASEADDR 0xE000E100UL /* Interrupt Set-Enable Registers */
#define NVIC_ICER_BASEADDR 0xE000E180UL /* Interrupt Clear-Enable Registers */
#define NVIC_IPR_BASEADDR 0xE000E400UL  /* Interrupt Priority Registers */

/* USART Taban Adresleri */
#define USART1_BASEADDR (APB2PERIPH_BASEADDR + 0x1000UL)
#define USART2_BASEADDR (APB1PERIPH_BASEADDR + 0x4400UL)

/* SPI Taban Adresleri */
#define SPI1_BASEADDR (APB2PERIPH_BASEADDR + 0x3000UL)
#define SPI2_BASEADDR (APB1PERIPH_BASEADDR + 0x3800UL)

/* I2C Taban Adresleri */
#define I2C1_BASEADDR (APB1PERIPH_BASEADDR + 0x5400UL)
#define I2C2_BASEADDR (APB1PERIPH_BASEADDR + 0x5800UL)

/* ========================================================================== */
/* 2. REGISTER YAPILARI (REGISTER STRUCT DEFINITIONS)                        */
/* ========================================================================== */

/**
 * @brief GPIO Register Haritası (Reference Manual Offset Sırasına Göre)
 */
typedef struct {
  volatile uint32_t MODER;   /* Mode Register (Offset: 0x00) */
  volatile uint32_t OTYPER;  /* Output Type Register (Offset: 0x04) */
  volatile uint32_t OSPEEDR; /* Output Speed Register (Offset: 0x08) */
  volatile uint32_t PUPDR;   /* Pull-up/Pull-down Register (Offset: 0x0C) */
  volatile uint32_t IDR;     /* Input Data Register (Offset: 0x10) */
  volatile uint32_t ODR;     /* Output Data Register (Offset: 0x14) */
  volatile uint32_t BSRR;    /* Bit Set/Reset Register (Offset: 0x18) */
  volatile uint32_t LCKR;    /* Configuration Lock Register (Offset: 0x1C) */
  volatile uint32_t
      AFR[2]; /* Alternate Function Low/High Registers (Offset: 0x20 - 0x24) */
} GPIO_RegDef_t;

/**
 * @brief RCC (Reset and Clock Control) Register Haritası
 */
typedef struct {
  volatile uint32_t CR;      /* Clock Control Register (Offset: 0x00) */
  volatile uint32_t PLLCFGR; /* PLL Configuration Register (Offset: 0x04) */
  volatile uint32_t CFGR;    /* Clock Configuration Register (Offset: 0x08) */
  volatile uint32_t CIR;     /* Clock Interrupt Register (Offset: 0x0C) */
  volatile uint32_t
      AHB1RSTR; /* AHB1 Peripheral Reset Register (Offset: 0x10) */
  volatile uint32_t
      AHB2RSTR; /* AHB2 Peripheral Reset Register (Offset: 0x14) */
  volatile uint32_t
      AHB3RSTR;       /* AHB3 Peripheral Reset Register (Offset: 0x18) */
  uint32_t RESERVED0; /* Ayrılmış Bellek Alanı (Offset: 0x1C) */
  volatile uint32_t
      APB1RSTR; /* APB1 Peripheral Reset Register (Offset: 0x20) */
  volatile uint32_t
      APB2RSTR;          /* APB2 Peripheral Reset Register (Offset: 0x24) */
  uint32_t RESERVED1[2]; /* Offset: 0x28 - 0x2C */
  volatile uint32_t
      AHB1ENR; /* AHB1 Peripheral Clock Enable Register (Offset: 0x30) */
  volatile uint32_t
      AHB2ENR; /* AHB2 Peripheral Clock Enable Register (Offset: 0x34) */
  volatile uint32_t
      AHB3ENR;        /* AHB3 Peripheral Clock Enable Register (Offset: 0x38) */
  uint32_t RESERVED2; /* Offset: 0x3C */
  volatile uint32_t
      APB1ENR; /* APB1 Peripheral Clock Enable Register (Offset: 0x40) */
  volatile uint32_t
      APB2ENR; /* APB2 Peripheral Clock Enable Register (Offset: 0x44) */
} RCC_RegDef_t;

/**
 * @brief EXTI (External Interrupt) Register Haritası
 */
typedef struct {
  volatile uint32_t IMR;  /* Interrupt Mask Register (Offset: 0x00) */
  volatile uint32_t EMR;  /* Event Mask Register (Offset: 0x04) */
  volatile uint32_t RTSR; /* Rising Trigger Selection Register (Offset: 0x08) */
  volatile uint32_t
      FTSR; /* Falling Trigger Selection Register (Offset: 0x0C) */
  volatile uint32_t
      SWIER;            /* Software Interrupt Event Register (Offset: 0x10) */
  volatile uint32_t PR; /* Pending Register (Offset: 0x14) */
} EXTI_RegDef_t;

/**
 * @brief SYSCFG (System Configuration Controller) Register Haritası
 */
typedef struct {
  volatile uint32_t MEMRMP; /* Memory Remap Register (Offset: 0x00) */
  volatile uint32_t PMC;    /* Peripheral Mode Configuration (Offset: 0x04) */
  volatile uint32_t
      EXTICR[4]; /* EXTI Configuration Registers 1-4 (Offset: 0x08-0x14) */
  uint32_t RESERVED[2];
  volatile uint32_t
      CMPCR; /* Compensation Cell Control Register (Offset: 0x20) */
} SYSCFG_RegDef_t;

/**
 * @brief USART Register Haritası
 */
typedef struct {
  volatile uint32_t SR;   /* Status Register (Offset: 0x00) */
  volatile uint32_t DR;   /* Data Register (Offset: 0x04) */
  volatile uint32_t BRR;  /* Baud Rate Register (Offset: 0x08) */
  volatile uint32_t CR1;  /* Control Register 1 (Offset: 0x0C) */
  volatile uint32_t CR2;  /* Control Register 2 (Offset: 0x10) */
  volatile uint32_t CR3;  /* Control Register 3 (Offset: 0x14) */
  volatile uint32_t GTPR; /* Guard Time and Prescaler Register (Offset: 0x18) */
} USART_RegDef_t;

/**
 * @brief SPI Register Haritası
 */
typedef struct {
  volatile uint32_t CR1;     /* Control Register 1 (Offset: 0x00) */
  volatile uint32_t CR2;     /* Control Register 2 (Offset: 0x04) */
  volatile uint32_t SR;      /* Status Register (Offset: 0x08) */
  volatile uint32_t DR;      /* Data Register (Offset: 0x0C) */
  volatile uint32_t CRCPR;   /* CRC Polynomial Register (Offset: 0x10) */
  volatile uint32_t RXCRCR;  /* RX CRC Register (Offset: 0x14) */
  volatile uint32_t TXCRCR;  /* TX CRC Register (Offset: 0x18) */
  volatile uint32_t I2SCFGR; /* I2S Configuration Register (Offset: 0x1C) */
  volatile uint32_t I2SPR;   /* I2S Prescaler Register (Offset: 0x20) */
} SPI_RegDef_t;

/**
 * @brief I2C Register Haritası
 */
typedef struct {
  volatile uint32_t CR1;   /* Control Register 1 (Offset: 0x00) */
  volatile uint32_t CR2;   /* Control Register 2 (Offset: 0x04) */
  volatile uint32_t OAR1;  /* Own Address Register 1 (Offset: 0x08) */
  volatile uint32_t OAR2;  /* Own Address Register 2 (Offset: 0x0C) */
  volatile uint32_t DR;    /* Data Register (Offset: 0x10) */
  volatile uint32_t SR1;   /* Status Register 1 (Offset: 0x14) */
  volatile uint32_t SR2;   /* Status Register 2 (Offset: 0x18) */
  volatile uint32_t CCR;   /* Clock Control Register (Offset: 0x1C) */
  volatile uint32_t TRISE; /* TRISE Register (Offset: 0x20) */
  volatile uint32_t FLTR;  /* FLTR Register (Offset: 0x24) */
} I2C_RegDef_t;

/* ========================================================================== */
/* 3. PERIPHERAL POINTER MAKROLARI                                           */
/* ========================================================================== */

#define GPIOA ((GPIO_RegDef_t *)GPIOA_BASEADDR)
#define GPIOB ((GPIO_RegDef_t *)GPIOB_BASEADDR)
#define GPIOC ((GPIO_RegDef_t *)GPIOC_BASEADDR)
#define GPIOD ((GPIO_RegDef_t *)GPIOD_BASEADDR)

#define RCC ((RCC_RegDef_t *)RCC_BASEADDR)

/* Pointer Makroları */
#define EXTI ((EXTI_RegDef_t *)EXTI_BASEADDR)
#define SYSCFG ((SYSCFG_RegDef_t *)SYSCFG_BASEADDR)

/* APB2 Clock Enable Makrosu (SYSCFG için gerekli) */
#define RCC_SYSCFG_CLK_EN() (RCC->APB2ENR |= (1U << 14))

/* IRQ (Interrupt Request) Numaraları - STM32F401/F411 için EXTI15_10 hattı */
#define IRQ_NO_EXTI15_10 40U

#define USART1 ((USART_RegDef_t *)USART1_BASEADDR)
#define USART2 ((USART_RegDef_t *)USART2_BASEADDR)

/* Clock Enable Makroları */
#define RCC_USART1_CLK_EN() (RCC->APB2ENR |= (1U << 4))
#define RCC_USART2_CLK_EN() (RCC->APB1ENR |= (1U << 17))

#define SPI1 ((SPI_RegDef_t *)SPI1_BASEADDR)
#define SPI2 ((SPI_RegDef_t *)SPI2_BASEADDR)

/* Clock Enable Makroları */
#define RCC_SPI1_CLK_EN() (RCC->APB2ENR |= (1U << 12))
#define RCC_SPI2_CLK_EN() (RCC->APB1ENR |= (1U << 14))

#define I2C1 ((I2C_RegDef_t *)I2C1_BASEADDR)
#define I2C2 ((I2C_RegDef_t *)I2C2_BASEADDR)

/* Clock Enable Makroları */
#define RCC_I2C1_CLK_EN() (RCC->APB1ENR |= (1U << 21))
#define RCC_I2C2_CLK_EN() (RCC->APB1ENR |= (1U << 22))

#endif /* STM32F4XX_H */