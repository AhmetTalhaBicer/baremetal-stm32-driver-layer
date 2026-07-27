#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "stm32f4xx.h"
#include <stddef.h>

/* I2C Hız Modları */
#define I2C_SCL_SPEED_SM    100000U /* Standard Mode (100 kHz) */
#define I2C_SCL_SPEED_FM    400000U /* Fast Mode (400 kHz) */

/* I2C Status Enum */
typedef enum {
    I2C_STATUS_OK = 0U,
    I2C_STATUS_BUSY_ERROR,
    I2C_STATUS_TIMEOUT,
    I2C_STATUS_NACK
} I2C_Status_t;

/* I2C Configuration Structure */
typedef struct {
    uint32_t I2C_SCLSpeed; /* 100kHz / 400kHz */
    uint8_t  I2C_AckControl;/* 1 = ACK Enable, 0 = NACK */
} I2C_Config_t;

/* Driver APIs */
void I2C_Init(I2C_RegDef_t *pI2Cx, const I2C_Config_t *pI2CConfig, uint32_t pclk1Freq);
I2C_Status_t I2C_MasterSendData(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr, const uint8_t *pTxBuf, uint32_t len, uint32_t timeout);
I2C_Status_t I2C_MasterReceiveData(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr, uint8_t *pRxBuf, uint32_t len, uint32_t timeout);
I2C_Status_t I2C_ReadRegister(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr, uint8_t regAddr, uint8_t *pData, uint32_t timeout);

#endif /* I2C_DRIVER_H */
