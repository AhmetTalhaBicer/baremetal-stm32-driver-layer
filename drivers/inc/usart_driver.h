/**
 * @file usart_driver.h
 * @brief USART (Universal Synchronous Asynchronous Receiver Transmitter) driver.
 * @author Antigravity
 * @date 2026-07-27
 */

#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include "stm32f4xx.h"
#include <stddef.h>

/**
 * @brief USART mode settings (Transmission, Reception, or Both).
 */
typedef enum {
  USART_MODE_ONLY_TX = 0U, /**< Only Transmitter enabled */
  USART_MODE_ONLY_RX = 1U, /**< Only Receiver enabled */
  USART_MODE_TXRX = 2U     /**< Both Transmitter and Receiver enabled */
} USART_Mode_t;

/**
 * @brief USART Configuration Structure.
 */
typedef struct {
  uint32_t USART_BaudRate;     /**< Baud Rate value (e.g. 115200) */
  USART_Mode_t USART_Mode;     /**< Operational mode setting */
  uint8_t USART_NoOfStopBits;  /**< Number of stop bits (e.g. 1 or 2) */
  uint8_t USART_WordLength;    /**< Word length setting (e.g. 8 bits or 9 bits) */
  uint8_t USART_ParityControl; /**< Parity control setting */
} USART_Config_t;

/* API Fonksiyonları */

/**
 * @brief Initializes the USART peripheral with the given configurations and calculates baudrate registers.
 * @param pUSARTx Pointer to the base address of the USART peripheral (USART1, USART2, etc.)
 * @param pUSARTConfig Pointer to the configuration structure
 * @param pclkFreq Frequency of the clock source feeding the USART peripheral
 */
void USART_Init(USART_RegDef_t *pUSARTx, const USART_Config_t *pUSARTConfig,
                uint32_t pclkFreq);

/**
 * @brief Sends data buffer over the USART interface in polling mode.
 * @param pUSARTx Pointer to the base address of the USART peripheral
 * @param pTxBuffer Pointer to the transmission byte array
 * @param len Size of the transmission byte array in bytes
 */
void USART_SendData(USART_RegDef_t *pUSARTx, const uint8_t *pTxBuffer,
                    uint32_t len);

/**
 * @brief Receives data buffer over the USART interface in polling mode.
 * @param pUSARTx Pointer to the base address of the USART peripheral
 * @param pRxBuffer Pointer to the receive buffer byte array
 * @param len Expected number of bytes to receive
 */
void USART_ReceiveData(USART_RegDef_t *pUSARTx, uint8_t *pRxBuffer,
                       uint32_t len);

/**
 * @brief Computes CRC-16 CCITT-FALSE (Polynomial: 0x1021, Initial: 0xFFFF) for data packet.
 * @param pData Pointer to the byte array to calculate CRC for
 * @param len Number of bytes to calculate CRC for
 * @return uint16_t The calculated CRC-16 checksum
 */
uint16_t CRC16_Calculate(const uint8_t *pData, size_t len);

#endif /* USART_DRIVER_H */