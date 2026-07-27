#include <stddef.h>
#include <stdbool.h>
#include "../inc/gpio_driver.h"

void GPIO_Init(GPIO_RegDef_t *pGPIOx, const GPIO_PinConfig_t *pPinConfig) {
    uint32_t temp = 0U;

    if ((pGPIOx == NULL) || (pPinConfig == NULL)) {
        return;
    }

    /* 1. Normal Mode / Interrupt Mode Ayrımı */
    if (pPinConfig->GPIO_PinMode <= GPIO_MODE_ANALOG) {
        temp = pGPIOx->MODER;
        temp &= ~(0x03U << (2U * pPinConfig->GPIO_PinNumber));
        temp |= (pPinConfig->GPIO_PinMode << (2U * pPinConfig->GPIO_PinNumber));
        pGPIOx->MODER = temp;
    } else {
        /* EXTI Modu Konfigürasyonu */
        /* a) Pin modu Giriş (Input) yapılmalı */
        pGPIOx->MODER &= ~(0x03U << (2U * pPinConfig->GPIO_PinNumber));

        /* b) Tetikleme Türü Ayarı (Falling / Rising) */
        if (pPinConfig->GPIO_PinMode == GPIO_MODE_IT_FT) {
            EXTI->FTSR |= (1U << pPinConfig->GPIO_PinNumber);
            EXTI->RTSR &= ~(1U << pPinConfig->GPIO_PinNumber);
        } else if (pPinConfig->GPIO_PinMode == GPIO_MODE_IT_RT) {
            EXTI->RTSR |= (1U << pPinConfig->GPIO_PinNumber);
            EXTI->FTSR &= ~(1U << pPinConfig->GPIO_PinNumber);
        } else if (pPinConfig->GPIO_PinMode == GPIO_MODE_IT_RFT) {
            EXTI->RTSR |= (1U << pPinConfig->GPIO_PinNumber);
            EXTI->FTSR |= (1U << pPinConfig->GPIO_PinNumber);
        }

        /* c) SYSCFG ile GPIO Portunu EXTI Hattına Bağlama */
        uint8_t extiRegIndex = pPinConfig->GPIO_PinNumber / 4U;
        uint8_t extiBitOffset = (pPinConfig->GPIO_PinNumber % 4U) * 4U;
        
        uint8_t portCode = 0U;
        if (pGPIOx == GPIOA) portCode = 0U;
        else if (pGPIOx == GPIOB) portCode = 1U;
        else if (pGPIOx == GPIOC) portCode = 2U;
        else if (pGPIOx == GPIOD) portCode = 3U;

        RCC_SYSCFG_CLK_EN();
        SYSCFG->EXTICR[extiRegIndex] &= ~(0x0FU << extiBitOffset);
        SYSCFG->EXTICR[extiRegIndex] |= (portCode << extiBitOffset);

        /* d) EXTI Kesme Maskesini Aç (Interrupt Mask Register Enable) */
        EXTI->IMR |= (1U << pPinConfig->GPIO_PinNumber);
    }

    /* 2. Alternate Function Konfigürasyonu (MODER = ALTFN durumunda geçerlidir) */
    if (pPinConfig->GPIO_PinMode == GPIO_MODE_ALTFN) {
        uint8_t temp_afr = pPinConfig->GPIO_PinNumber / 8U;
        uint8_t temp_pin = pPinConfig->GPIO_PinNumber % 8U;
        pGPIOx->AFR[temp_afr] &= ~(0x0FU << (4U * temp_pin));
        pGPIOx->AFR[temp_afr] |= (pPinConfig->GPIO_PinAltFunMode << (4U * temp_pin));
    }

    /* 3. Hız Konfigürasyonu (OSPEEDR) */
    temp = pGPIOx->OSPEEDR;
    temp &= ~(0x03U << (2U * pPinConfig->GPIO_PinNumber));
    temp |= (pPinConfig->GPIO_PinSpeed << (2U * pPinConfig->GPIO_PinNumber));
    pGPIOx->OSPEEDR = temp;

    /* 4. Pull-up / Pull-down Ayarı (PUPDR) */
    temp = pGPIOx->PUPDR;
    temp &= ~(0x03U << (2U * pPinConfig->GPIO_PinNumber));
    temp |= (pPinConfig->GPIO_PinPuPdControl << (2U * pPinConfig->GPIO_PinNumber));
    pGPIOx->PUPDR = temp;

    /* 5. Çıkış Tipi Konfigürasyonu (OTYPER) */
    temp = pGPIOx->OTYPER;
    temp &= ~(0x01U << pPinConfig->GPIO_PinNumber);
    temp |= (pPinConfig->GPIO_PinOPType << pPinConfig->GPIO_PinNumber);
    pGPIOx->OTYPER = temp;
}

void GPIO_WritePin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber, uint8_t value) {
    if (value != 0U) {
        /* BSRR Register (Bit Set Reset Register): İlk 16 bit biti SET (1) yapar */
        pGPIOx->BSRR = (1U << pinNumber);
    } else {
        /* BSRR Register'ın üst 16 biti (16-31) biti RESET (0) yapar */
        pGPIOx->BSRR = (1U << (pinNumber + 16U));
    }
}

void GPIO_TogglePin(GPIO_RegDef_t *pGPIOx, uint8_t pinNumber) {
    pGPIOx->ODR ^= (1U << pinNumber);
}

uint8_t GPIO_ReadPin(const GPIO_RegDef_t *pGPIOx, uint8_t pinNumber) {
    return (uint8_t)((pGPIOx->IDR >> pinNumber) & 0x01U);
}

/* NVIC Kontrolü (Cortex-M Core Register Yapılandırması) */
void GPIO_IRQConfig(uint8_t irqNumber, uint8_t priority, uint8_t status) {
    if (status != 0U) {
        /* Interrupt Enable (ISER Register) */
        volatile uint32_t *pISER = (volatile uint32_t *)(uintptr_t)(NVIC_ISER_BASEADDR + ((irqNumber / 32U) * 4U));
        *pISER |= (1U << (irqNumber % 32U));
    } else {
        /* Interrupt Disable (ICER Register) */
        volatile uint32_t *pICER = (volatile uint32_t *)(uintptr_t)(NVIC_ICER_BASEADDR + ((irqNumber / 32U) * 4U));
        *pICER |= (1U << (irqNumber % 32U));
    }

    /* Priority Configuration (IPR Register) */
    volatile uint32_t *pIPR = (volatile uint32_t *)(uintptr_t)(NVIC_IPR_BASEADDR + ((irqNumber / 4U) * 4U));
    uint8_t iprOffset = (irqNumber % 4U) * 8U;
    
    /* STM32F4 serisinde öncelik değerinin sadece üst 4 biti (bits 4..7) kullanılır */
    *pIPR &= ~(0xFFU << iprOffset);
    *pIPR |= ((priority << 4U) << iprOffset);
}

/* ISR İçinden Çağrılan Hata/Bayrak Temizleme API'si */
void GPIO_IRQHandling(uint8_t pinNumber) {
    /* EXTI Pending Register (PR) kontrolü: Bayrak 1 ise temizlemek için RRC mantığı gereği '1' YAZILMALIDIR! */
    if ((EXTI->PR & (1U << pinNumber)) != 0U) {
        EXTI->PR |= (1U << pinNumber); /* Bayrağı temizle (Clear by writing 1) */
    }
}