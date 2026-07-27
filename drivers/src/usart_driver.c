#include "../inc/usart_driver.h"

void USART_Init(USART_RegDef_t *pUSARTx, const USART_Config_t *pUSARTConfig,
                uint32_t pclkFreq) {
  uint32_t tempreg = 0U;

  if ((pUSARTx == NULL) || (pUSARTConfig == NULL) || (pclkFreq == 0U)) {
    return;
  }

  /* 1. Mod Konfigürasyonu (CR1 - TX / RX Enable) */
  if (pUSARTConfig->USART_Mode == USART_MODE_ONLY_RX) {
    tempreg |= (1U << 2U); /* RE (Receiver Enable) */
  } else if (pUSARTConfig->USART_Mode == USART_MODE_ONLY_TX) {
    tempreg |= (1U << 3U); /* TE (Transmitter Enable) */
  } else if (pUSARTConfig->USART_Mode == USART_MODE_TXRX) {
    tempreg |= (1U << 2U) | (1U << 3U);
  }

  /* 2. Baud Rate Hesaplama ve BRR Register Yazımı */
  /* USARTDIV = pclkFreq / (16 * BaudRate) */
  /* Hassas integer bölme için 100 ile çarpıp yuvarlama mantığı kullanıyoruz */
  uint32_t usartdiv = ((25U * pclkFreq) / (4U * pUSARTConfig->USART_BaudRate));
  uint32_t mantissa = usartdiv / 100U;
  uint32_t fraction = (((usartdiv - (mantissa * 100U)) * 16U) + 50U) / 100U;

  pUSARTx->BRR = ((mantissa << 4U) | (fraction & 0x0FU));

  /* 3. CR1 Register Güncelleme ve USART Enable (UE bit 13) */
  tempreg |= (1U << 13U);
  pUSARTx->CR1 = tempreg;
}

void USART_SendData(USART_RegDef_t *pUSARTx, const uint8_t *pTxBuffer,
                    uint32_t len) {
  for (uint32_t i = 0U; i < len; i++) {
    /* TXE (Transmit Data Register Empty - Bit 7) bayrağının '1' olmasını bekle
     */
    while (!(pUSARTx->SR & (1U << 7U))) {
      /* Polling - Donanım hazır olana kadar bekle */
    }
    /* Veriyi Data Register'a yaz */
    pUSARTx->DR = (pTxBuffer[i] & 0xFFU);
  }

  /* Tüm baytların fiziksel hattan tamamen çıkması için TC (Transmission
   * Complete - Bit 6) bekle */
  while (!(pUSARTx->SR & (1U << 6U))) {
    /* Wait for TC */
  }
}

void USART_ReceiveData(USART_RegDef_t *pUSARTx, uint8_t *pRxBuffer,
                       uint32_t len) {
  for (uint32_t i = 0U; i < len; i++) {
    /* RXNE (Read Data Register Not Empty - Bit 5) bayrağının '1' olmasını bekle
     */
    while (!(pUSARTx->SR & (1U << 5U))) {
      /* Polling */
    }
    pRxBuffer[i] = (uint8_t)(pUSARTx->DR & 0xFFU);
  }
}

/* Havacılık ve Sanayi Standartı CRC-16/CCITT-FALSE (Polynomial: 0x1021) */
uint16_t CRC16_Calculate(const uint8_t *pData, size_t len) {
  uint16_t crc = 0xFFFFU;

  for (size_t i = 0; i < len; i++) {
    crc ^= ((uint16_t)pData[i] << 8U);
    for (uint8_t bit = 0; bit < 8U; bit++) {
      if ((crc & 0x8000U) != 0U) {
        crc = (crc << 1U) ^ 0x1021U;
      } else {
        crc <<= 1U;
      }
    }
  }
  return crc;
}