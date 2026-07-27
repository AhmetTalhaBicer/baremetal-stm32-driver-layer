#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include "stm32f4xx.h"
#include <stddef.h>

/* USART Modları */
typedef enum {
  USART_MODE_ONLY_TX = 0U,
  USART_MODE_ONLY_RX = 1U,
  USART_MODE_TXRX = 2U
} USART_Mode_t;

/* USART Konfigürasyon Yapısı */
typedef struct {
  uint32_t USART_BaudRate; /* Örn: 115200 */
  USART_Mode_t USART_Mode;
  uint8_t USART_NoOfStopBits;  /* 1 Stop Bit */
  uint8_t USART_WordLength;    /* 8 Bits Data */
  uint8_t USART_ParityControl; /* No Parity */
} USART_Config_t;

/* API Fonksiyonları */
void USART_Init(USART_RegDef_t *pUSARTx, const USART_Config_t *pUSARTConfig,
                uint32_t pclkFreq);
void USART_SendData(USART_RegDef_t *pUSARTx, const uint8_t *pTxBuffer,
                    uint32_t len);
void USART_ReceiveData(USART_RegDef_t *pUSARTx, uint8_t *pRxBuffer,
                       uint32_t len);

/* CRC-16 (CCITT-FALSE: Poly 0x1021, Init 0xFFFF) Hesaplama */
uint16_t CRC16_Calculate(const uint8_t *pData, size_t len);

#endif /* USART_DRIVER_H */