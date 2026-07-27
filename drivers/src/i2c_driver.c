#include "../inc/i2c_driver.h"

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx) {
    pI2Cx->CR1 |= (1U << 8U); /* START Bit = 1 */
}

static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx) {
    pI2Cx->CR1 |= (1U << 9U); /* STOP Bit = 1 */
}

static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx) {
    /* STM32 Reference Manual: ADDR bayrağını temizlemek için SR1 okunur, ardından SR2 okunur */
    volatile uint32_t dummyRead = pI2Cx->SR1;
    dummyRead = pI2Cx->SR2;
    (void)dummyRead;
}

void I2C_Init(I2C_RegDef_t *pI2Cx, const I2C_Config_t *pI2CConfig, uint32_t pclk1Freq) {
    if ((pI2Cx == NULL) || (pI2CConfig == NULL) || (pclk1Freq == 0U)) {
        return;
    }

    /* 1. Peripheral Clock Frekansını CR2 Register'ına Yaz (MHz cinsinden) */
    uint32_t pclkMHz = pclk1Freq / 1000000U;
    pI2Cx->CR2 = (pclkMHz & 0x3FU);

    /* 2. Clock Control Register (CCR) Hesaplama (100kHz Standard Mode) */
    /* T_high = CCR * T_pclk1 => CCR = pclk1 / (2 * SCL_Speed) */
    uint16_t ccrValue = (uint16_t)(pclk1Freq / (2U * pI2CConfig->I2C_SCLSpeed));
    pI2Cx->CCR = (ccrValue & 0xFFFU);

    /* 3. TRISE Register Hesaplama (Standard Mode max rise time = 1000ns) */
    pI2Cx->TRISE = (pclkMHz + 1U) & 0x3FU;

    /* 4. ACK Control & Peripheral Enable (PE) */
    if (pI2CConfig->I2C_AckControl) {
        pI2Cx->CR1 |= (1U << 10U); /* ACK Enable */
    }
    pI2Cx->CR1 |= (1U << 0U);     /* PE = 1 (Enable I2C) */
}

I2C_Status_t I2C_MasterSendData(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr, const uint8_t *pTxBuf, uint32_t len, uint32_t timeout) {
    uint32_t timeoutCount = 0U;

    /* 1. START Condition Üret */
    I2C_GenerateStartCondition(pI2Cx);

    /* SB (Start Bit) Bayrağını Bekle (SR1 Bit 0) */
    while (!(pI2Cx->SR1 & (1U << 0U))) {
        if (++timeoutCount > timeout) return I2C_STATUS_TIMEOUT;
    }

    /* 2. Slave Adresini + Write Biti (0) Gönder */
    pI2Cx->DR = (slaveAddr << 1U) & ~(0x01U);

    /* ADDR (Address Sent) Bayrağını Bekle (SR1 Bit 1) */
    timeoutCount = 0U;
    while (!(pI2Cx->SR1 & (1U << 1U))) {
        /* AF (Acknowledge Failure) Kontrolü */
        if (pI2Cx->SR1 & (1U << 10U)) {
            pI2Cx->SR1 &= ~(1U << 10U); /* Clear AF */
            I2C_GenerateStopCondition(pI2Cx);
            return I2C_STATUS_NACK;
        }
        if (++timeoutCount > timeout) return I2C_STATUS_TIMEOUT;
    }

    /* ADDR Bayrağını Temizle */
    I2C_ClearADDRFlag(pI2Cx);

    /* 3. Verileri Teker Teker Gönder */
    for (uint32_t i = 0U; i < len; i++) {
        /* TXE (Data Register Empty - Bit 7) Bekle */
        timeoutCount = 0U;
        while (!(pI2Cx->SR1 & (1U << 7U))) {
            if (++timeoutCount > timeout) return I2C_STATUS_TIMEOUT;
        }
        pI2Cx->DR = pTxBuf[i];
    }

    /* BTF (Byte Transfer Finished - Bit 2) Bekle */
    timeoutCount = 0U;
    while (!(pI2Cx->SR1 & (1U << 2U))) {
        if (++timeoutCount > timeout) return I2C_STATUS_TIMEOUT;
    }

    /* 4. STOP Condition Üret */
    I2C_GenerateStopCondition(pI2Cx);
    return I2C_STATUS_OK;
}

I2C_Status_t I2C_MasterReceiveData(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr, uint8_t *pRxBuf, uint32_t len, uint32_t timeout) {
    uint32_t timeoutCount = 0U;

    /* 1. START Condition Üret */
    I2C_GenerateStartCondition(pI2Cx);

    while (!(pI2Cx->SR1 & (1U << 0U))) {
        if (++timeoutCount > timeout) return I2C_STATUS_TIMEOUT;
    }

    /* 2. Slave Adresini + Read Biti (1) Gönder */
    pI2Cx->DR = (slaveAddr << 1U) | 0x01U;

    timeoutCount = 0U;
    while (!(pI2Cx->SR1 & (1U << 1U))) {
        if (++timeoutCount > timeout) return I2C_STATUS_TIMEOUT;
    }

    if (len == 1U) {
        /* Tek bayt okunacaksa ADDR temizlenmeden önce ACK kapatılmalı ve STOP verilmeli! */
        pI2Cx->CR1 &= ~(1U << 10U); /* Disable ACK */
        I2C_ClearADDRFlag(pI2Cx);
        I2C_GenerateStopCondition(pI2Cx);

        /* RXNE (Bit 6) Bekle */
        timeoutCount = 0U;
        while (!(pI2Cx->SR1 & (1U << 6U))) {
            if (++timeoutCount > timeout) return I2C_STATUS_TIMEOUT;
        }
        pRxBuf[0] = (uint8_t)(pI2Cx->DR & 0xFFU);
    } else {
        I2C_ClearADDRFlag(pI2Cx);
        for (uint32_t i = 0U; i < len; i++) {
            if (i == len - 1U) {
                pI2Cx->CR1 &= ~(1U << 10U); /* Son bayttan önce ACK kapat */
                I2C_GenerateStopCondition(pI2Cx);
            }
            timeoutCount = 0U;
            while (!(pI2Cx->SR1 & (1U << 6U))) {
                if (++timeoutCount > timeout) return I2C_STATUS_TIMEOUT;
            }
            pRxBuf[i] = (uint8_t)(pI2Cx->DR & 0xFFU);
        }
    }

    /* ACK'i tekrar aktif et (Sonraki işlemler için) */
    pI2Cx->CR1 |= (1U << 10U);
    return I2C_STATUS_OK;
}

I2C_Status_t I2C_ReadRegister(I2C_RegDef_t *pI2Cx, uint8_t slaveAddr, uint8_t regAddr, uint8_t *pData, uint32_t timeout) {
    I2C_Status_t status = I2C_MasterSendData(pI2Cx, slaveAddr, &regAddr, 1U, timeout);
    if (status != I2C_STATUS_OK) return status;
    return I2C_MasterReceiveData(pI2Cx, slaveAddr, pData, 1U, timeout);
}
