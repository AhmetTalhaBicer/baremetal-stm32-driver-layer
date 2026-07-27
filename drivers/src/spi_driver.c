#include "../inc/spi_driver.h"

void SPI_Init(SPI_RegDef_t *pSPIx, const SPI_Config_t *pSPIConfig) {
  uint32_t tempreg = 0U;

  if ((pSPIx == NULL) || (pSPIConfig == NULL)) {
    return;
  }

  /* 1. Device Mode (Master / Slave) -> CR1 Bit 2 (MSTR) */
  tempreg |= (pSPIConfig->SPI_DeviceMode << 2U);

  /* 2. CPOL & CPHA -> CR1 Bit 1 & Bit 0 */
  tempreg |= (pSPIConfig->SPI_CPOL << 1U);
  tempreg |= (pSPIConfig->SPI_CPHA << 0U);

  /* 3. Baud Rate Prescaler -> CR1 Bits [5:3] (BR) */
  tempreg |= (pSPIConfig->SPI_Speed << 3U);

  /* 4. Software Slave Management (SSM & SSI) -> Donanımsal CS hatalarını
   * önlemek için SSM=1 yapıyoruz */
  tempreg |= (1U << 9U); /* SSM = 1 (Software Slave Management Enable) */
  tempreg |= (1U << 8U); /* SSI = 1 (Internal Slave Select) */

  /* 5. Data Frame Format (8-bit / 16-bit) -> CR1 Bit 11 */
  tempreg |= (pSPIConfig->SPI_DFF << 11U);

  /* 6. SPI Peripheral Enable -> CR1 Bit 6 (SPE) */
  tempreg |= (1U << 6U);

  pSPIx->CR1 = tempreg;
}

/* Full-Duplex Veri Gönderim ve Alım Fonksiyonu (Timeout Korumalı) */
SPI_Status_t SPI_TransmitReceive(SPI_RegDef_t *pSPIx, const uint8_t *pTxData,
                                 uint8_t *pRxData, uint32_t size,
                                 uint32_t timeout) {
  uint32_t timeoutCount = 0U;

  for (uint32_t i = 0U; i < size; i++) {
    /* TXE (Transmit Buffer Empty - Bit 1) Bayrağını Bekle */
    timeoutCount = 0U;
    while (!(pSPIx->SR & (1U << 1U))) {
      if (++timeoutCount > timeout)
        return SPI_STATUS_TIMEOUT;
    }

    /* Veriyi Gönder (Kukla/Dummy bayt veya gerçek veri) */
    pSPIx->DR = (pTxData != NULL) ? pTxData[i] : 0xFFU;

    /* RXNE (Receive Buffer Not Empty - Bit 0) Bayrağını Bekle */
    timeoutCount = 0U;
    while (!(pSPIx->SR & (1U << 0U))) {
      if (++timeoutCount > timeout)
        return SPI_STATUS_TIMEOUT;
    }

    /* Gelen Veriyi Oku */
    uint8_t dummyOrData = (uint8_t)(pSPIx->DR & 0xFFU);
    if (pRxData != NULL) {
      pRxData[i] = dummyOrData;
    }
  }

  /* Bus Meşguliyet Kontrolü (BSY - Bit 7) */
  timeoutCount = 0U;
  while (pSPIx->SR & (1U << 7U)) {
    if (++timeoutCount > timeout)
      return SPI_STATUS_BUSY_ERROR;
  }

  return SPI_STATUS_OK;
}

/* IMU Sensör Register Okuma Makrosu (Read Bit = 0x80) */
uint8_t SPI_ReadRegister(SPI_RegDef_t *pSPIx, GPIO_RegDef_t *pCS_Port,
                         uint8_t csPin, uint8_t regAddr) {
  uint8_t txBuf[2];
  uint8_t rxBuf[2];

  /* SPI IMU Okuma Protokolü: Adresin en anlamlı biti (MSB / Bit 7) '1'
   * YAPILMALIDIR! */
  txBuf[0] = regAddr | 0x80U; /* Read Flag Set */
  txBuf[1] = 0xFFU; /* Dummy Byte (Sensörün veriyi göndermesi için gerekli saat
                       vurumu) */

  /* 1. CS Pinini Lojik 0 Yap (Active Low - Sensörü Seç) */
  pCS_Port->BSRR = (1U << (csPin + 16U));

  /* 2. 2-Baytlık Veriyi Aktar */
  SPI_TransmitReceive(pSPIx, txBuf, rxBuf, 2U, 10000U);

  /* 3. CS Pinini Lojik 1 Yap (Sensör Seçimini Kaldır) */
  pCS_Port->BSRR = (1U << csPin);

  return rxBuf[1]; /* İkinci bayt sensörün gönderdiği register içeriğidir */
}

/* IMU Sensör Register Yazma Makrosu (Write Bit = 0x00) */
void SPI_WriteRegister(SPI_RegDef_t *pSPIx, GPIO_RegDef_t *pCS_Port,
                       uint8_t csPin, uint8_t regAddr, uint8_t value) {
  uint8_t txBuf[2];

  txBuf[0] = regAddr & ~(0x80U); /* Write Flag (MSB = 0) */
  txBuf[1] = value;

  /* CS Low */
  pCS_Port->BSRR = (1U << (csPin + 16U));

  SPI_TransmitReceive(pSPIx, txBuf, NULL, 2U, 10000U);

  /* CS High */
  pCS_Port->BSRR = (1U << csPin);
}