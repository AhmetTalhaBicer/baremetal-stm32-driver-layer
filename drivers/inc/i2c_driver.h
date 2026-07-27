/**
 * @file i2c_driver.h
 * @brief I2C (Inter-Integrated Circuit) master driver for STM32F4xx microcontrollers.
 * @author Antigravity
 * @date 2026-07-27
 */

#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "stm32f4xx.h"
#include <stddef.h>

/* I2C Hız Modları */
#define I2C_SCL_SPEED_SM    100000U /**< Standard Mode speed setting (100 kHz) */
#define I2C_SCL_SPEED_FM    400000U /**< Fast Mode speed setting (400 kHz) */

/**
 * @brief I2C operational status codes.
 */
typedef enum {
    I2C_STATUS_OK = 0U,       /**< Operation completed successfully */
    I2C_STATUS_BUSY_ERROR,    /**< Bus is busy or not responding */
    I2C_STATUS_TIMEOUT,       /**< Operation timed out */
    I2C_STATUS_NACK           /**< Received NACK from slave device */
} I2C_Status_t;

/**
 * @brief I2C Configuration Structure.
 */
typedef struct {
    uint32_t I2C_SCLSpeed;    /**< SCL clock speed (e.g. I2C_SCL_SPEED_SM or I2C_SCL_SPEED_FM) */
    uint8_t  I2C_AckControl;  /**< 1 to enable ACK, 0 to disable ACK (send NACK) */
} I2C_Config_t;

/* Driver APIs */

/**
 * @brief Initializes the I2C peripheral with given configurations.
 * @param pI2Cx Pointer to the base address of the I2C peripheral (I2C1, I2C2, etc.)
 * @param pI2CConfig Pointer to the configuration structure
 * @param pclk1Freq Frequency of the APB1 clock line driving the I2C peripheral
 */
void I2C_Init(I2C_RegDef_t *pI2Cx, const I2C_Config_t *pI2CConfig, uint32_t pclk1Freq);

/**
 * @brief Transmits a byte buffer as I2C master.
 * @param pI2Cx Pointer to the base address of the I2C peripheral
 * @param slaveAddr The 7-bit slave address of target device
 * @param pTxBuf Pointer to the transmission byte array
 * @param len Size of data to transmit in bytes
 * @param timeout Limit for loop iteration timeout protection
 * @return I2C_Status_t Operational status
 */
I2C_Status_t I2C_MasterSendData(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr, const uint8_t *pTxBuf, uint32_t len, uint32_t timeout);

/**
 * @brief Receives a byte buffer as I2C master.
 * @param pI2Cx Pointer to the base address of the I2C peripheral
 * @param slaveAddr The 7-bit slave address of target device
 * @param pRxBuf Pointer to the receive buffer byte array
 * @param len Expected number of bytes to receive
 * @param timeout Limit for loop iteration timeout protection
 * @return I2C_Status_t Operational status
 */
I2C_Status_t I2C_MasterReceiveData(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr, uint8_t *pRxBuf, uint32_t len, uint32_t timeout);

/**
 * @brief Reads a sensor register over I2C.
 * @param pI2Cx Pointer to the base address of the I2C peripheral
 * @param slaveAddr The 7-bit slave address of target device
 * @param regAddr The sensor register address to read
 * @param pData Pointer to the variable to store read value
 * @param timeout Limit for loop iteration timeout protection
 * @return I2C_Status_t Operational status
 */
I2C_Status_t I2C_ReadRegister(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr, uint8_t regAddr, uint8_t *pData, uint32_t timeout);

#endif /* I2C_DRIVER_H */
