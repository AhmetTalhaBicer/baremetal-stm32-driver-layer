#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include "stm32f4xx.h"
#include <stddef.h>

/* SPI Modları */
typedef enum {
  SPI_DEVICE_MODE_SLAVE = 0U,
  SPI_DEVICE_MODE_MASTER = 1U
} SPI_DeviceMode_t;

/* SPI CPOL ve CPHA */
typedef enum { SPI_CPOL_LOW = 0U, SPI_CPOL_HIGH = 1U } SPI_CPOL_t;

typedef enum { SPI_CPHA_LOW = 0U, SPI_CPHA_HIGH = 1U } SPI_CPHA_t;

/* SPI Clock Prescaler (Baud Rate) */
typedef enum {
  SPI_SPEED_DIV_2 = 0U,
  SPI_SPEED_DIV_4 = 1U,
  SPI_SPEED_DIV_8 = 2U,
  SPI_SPEED_DIV_16 = 3U,
  SPI_SPEED_DIV_32 = 4U,
  SPI_SPEED_DIV_64 = 5U,
  SPI_SPEED_DIV_128 = 6U,
  SPI_SPEED_DIV_256 = 7U
} SPI_Speed_t;

/* Configuration Structure */
typedef struct {
  SPI_DeviceMode_t SPI_DeviceMode;
  SPI_CPOL_t SPI_CPOL;
  SPI_CPHA_t SPI_CPHA;
  SPI_Speed_t SPI_Speed;
  uint8_t SPI_DFF; /* Data Frame Format: 0 = 8-bit, 1 = 16-bit */
} SPI_Config_t;

/* SPI Status Enum */
typedef enum {
  SPI_STATUS_OK = 0U,
  SPI_STATUS_TIMEOUT,
  SPI_STATUS_BUSY_ERROR
} SPI_Status_t;

/* Driver APIs */
void SPI_Init(SPI_RegDef_t *pSPIx, const SPI_Config_t *pSPIConfig);
SPI_Status_t SPI_TransmitReceive(SPI_RegDef_t *pSPIx, const uint8_t *pTxData,
                                 uint8_t *pRxData, uint32_t size,
                                 uint32_t timeout);
uint8_t SPI_ReadRegister(SPI_RegDef_t *pSPIx, GPIO_RegDef_t *pCS_Port,
                         uint8_t csPin, uint8_t regAddr);
void SPI_WriteRegister(SPI_RegDef_t *pSPIx, GPIO_RegDef_t *pCS_Port,
                       uint8_t csPin, uint8_t regAddr, uint8_t value);

#endif /* SPI_DRIVER_H */