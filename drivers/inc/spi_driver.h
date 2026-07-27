/**
 * @file spi_driver.h
 * @brief SPI (Serial Peripheral Interface) driver for STM32F4xx microcontrollers.
 * @author Antigravity
 * @date 2026-07-27
 */

#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include "stm32f4xx.h"
#include <stddef.h>

/**
 * @brief SPI operational modes (Master / Slave).
 */
typedef enum {
  SPI_DEVICE_MODE_SLAVE = 0U,  /**< SPI configured as slave */
  SPI_DEVICE_MODE_MASTER = 1U  /**< SPI configured as master */
} SPI_DeviceMode_t;

/**
 * @brief SPI Clock Polarity (CPOL).
 */
typedef enum { 
  SPI_CPOL_LOW = 0U,  /**< Clock idle state is low */
  SPI_CPOL_HIGH = 1U  /**< Clock idle state is high */
} SPI_CPOL_t;

/**
 * @brief SPI Clock Phase (CPHA).
 */
typedef enum { 
  SPI_CPHA_LOW = 0U,  /**< Data captured on first clock transition edge */
  SPI_CPHA_HIGH = 1U  /**< Data captured on second clock transition edge */
} SPI_CPHA_t;

/**
 * @brief SPI clock frequency prescalers.
 */
typedef enum {
  SPI_SPEED_DIV_2 = 0U,   /**< SCK frequency = Peripheral Clock / 2 */
  SPI_SPEED_DIV_4 = 1U,   /**< SCK frequency = Peripheral Clock / 4 */
  SPI_SPEED_DIV_8 = 2U,   /**< SCK frequency = Peripheral Clock / 8 */
  SPI_SPEED_DIV_16 = 3U,  /**< SCK frequency = Peripheral Clock / 16 */
  SPI_SPEED_DIV_32 = 4U,  /**< SCK frequency = Peripheral Clock / 32 */
  SPI_SPEED_DIV_64 = 5U,  /**< SCK frequency = Peripheral Clock / 64 */
  SPI_SPEED_DIV_128 = 6U, /**< SCK frequency = Peripheral Clock / 128 */
  SPI_SPEED_DIV_256 = 7U  /**< SCK frequency = Peripheral Clock / 256 */
} SPI_Speed_t;

/**
 * @brief SPI Configuration Structure.
 */
typedef struct {
  SPI_DeviceMode_t SPI_DeviceMode; /**< Device role (Master/Slave) */
  SPI_CPOL_t SPI_CPOL;             /**< Clock Polarity setting */
  SPI_CPHA_t SPI_CPHA;             /**< Clock Phase setting */
  SPI_Speed_t SPI_Speed;           /**< Clock frequency prescaler */
  uint8_t SPI_DFF;                 /**< Data Frame Format: 0 for 8-bit, 1 for 16-bit */
} SPI_Config_t;

/**
 * @brief SPI status/error codes.
 */
typedef enum {
  SPI_STATUS_OK = 0U,      /**< Operation completed successfully */
  SPI_STATUS_TIMEOUT,      /**< Loop timed out waiting for hardware flag */
  SPI_STATUS_BUSY_ERROR    /**< Bus busy or frame transmission error */
} SPI_Status_t;

/* Driver APIs */

/**
 * @brief Initializes the SPI peripheral with given configuration structures.
 * @param pSPIx Pointer to the base address of the SPI peripheral (SPI1, SPI2, etc.)
 * @param pSPIConfig Pointer to the configuration structure
 */
void SPI_Init(SPI_RegDef_t *pSPIx, const SPI_Config_t *pSPIConfig);

/**
 * @brief Transmits and receives data simultaneously (Full-Duplex) with timeout protection.
 * @param pSPIx Pointer to the base address of the SPI peripheral
 * @param pTxData Pointer to transmission buffer. Can be NULL to send dummy data.
 * @param pRxData Pointer to receive buffer. Can be NULL to ignore incoming data.
 * @param size Number of bytes to transfer
 * @param timeout Loop counter limit for timeout protection
 * @return SPI_Status_t Status of the operation
 */
SPI_Status_t SPI_TransmitReceive(SPI_RegDef_t *pSPIx, const uint8_t *pTxData,
                                 uint8_t *pRxData, uint32_t size,
                                 uint32_t timeout);

/**
 * @brief Reads an internal register of an SPI sensor (e.g. IMU).
 * @param pSPIx Pointer to the base address of the SPI peripheral
 * @param pCS_Port Pointer to the GPIO port base address of the Chip Select pin
 * @param csPin Pin number of the Chip Select pin (0 to 15)
 * @param regAddr The sensor register address to read
 * @return uint8_t The read register content
 */
uint8_t SPI_ReadRegister(SPI_RegDef_t *pSPIx, GPIO_RegDef_t *pCS_Port,
                         uint8_t csPin, uint8_t regAddr);

/**
 * @brief Writes a value to an internal register of an SPI sensor (e.g. IMU).
 * @param pSPIx Pointer to the base address of the SPI peripheral
 * @param pCS_Port Pointer to the GPIO port base address of the Chip Select pin
 * @param csPin Pin number of the Chip Select pin (0 to 15)
 * @param regAddr The sensor register address to write
 * @param value The value to write to the register
 */
void SPI_WriteRegister(SPI_RegDef_t *pSPIx, GPIO_RegDef_t *pCS_Port,
                       uint8_t csPin, uint8_t regAddr, uint8_t value);

#endif /* SPI_DRIVER_H */