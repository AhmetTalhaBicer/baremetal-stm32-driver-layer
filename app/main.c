#include "../drivers/inc/gpio_driver.h"
#include "../drivers/inc/rcc_driver.h"
#include "../drivers/inc/stm32f4xx.h"
#include "../drivers/inc/usart_driver.h"
#include "../drivers/inc/spi_driver.h"
#include "../drivers/inc/i2c_driver.h"
#include "./telemetry.h"

int main(void) {
    /* 1. Saat Sinyallerini Aç */
    RCC_GPIOA_CLK_EN();
    RCC_GPIOB_CLK_EN();
    RCC_USART2_CLK_EN();
    RCC_SPI1_CLK_EN();
    RCC_I2C1_CLK_EN();

    /* 2. USART2 Pinleri (PA2=TX, PA3=RX) Alternatif Fonksiyona (AF7) Ayarla */
    GPIO_PinConfig_t usartPins = {
        .GPIO_PinNumber = GPIO_PIN_2,
        .GPIO_PinMode = GPIO_MODE_ALTFN,
        .GPIO_PinSpeed = GPIO_SPEED_HIGH,
        .GPIO_PinOPType = GPIO_OP_TYPE_PP,
        .GPIO_PinPuPdControl = GPIO_PIN_PU,
        .GPIO_PinAltFunMode = 7U /* AF7 = USART2 */
    };
    GPIO_Init(GPIOA, &usartPins);
    usartPins.GPIO_PinNumber = GPIO_PIN_3;
    GPIO_Init(GPIOA, &usartPins);

    /* Alternatif Fonksiyon Register (AFR) Ayarı */
    GPIOA->AFR[0] &= ~((0x0FU << 8U) | (0x0FU << 12U));
    GPIOA->AFR[0] |= ((7U << 8U) | (7U << 12U)); /* PA2 ve PA3 için AF7 */

    /* USART2 Konfigürasyonu (115200 Baud, 16MHz APB1 Saat Sinyali) */
    USART_Config_t usartConfig = {
        .USART_BaudRate = 115200U,
        .USART_Mode = USART_MODE_TXRX,
        .USART_NoOfStopBits = 1U,
        .USART_WordLength = 8U,
        .USART_ParityControl = 0U
    };
    USART_Init(USART2, &usartConfig, 16000000U);

    /* 3. SPI1 Pinleri (PA5=SCK, PA6=MISO, PA7=MOSI) Alternatif Fonksiyon (AF5) */
    GPIO_PinConfig_t spiPins = {
        .GPIO_PinNumber = GPIO_PIN_5,
        .GPIO_PinMode = GPIO_MODE_ALTFN,
        .GPIO_PinSpeed = GPIO_SPEED_HIGH,
        .GPIO_PinOPType = GPIO_OP_TYPE_PP,
        .GPIO_PinPuPdControl = GPIO_NO_PUPD,
        .GPIO_PinAltFunMode = 5U /* AF5 = SPI1 */
    };
    GPIO_Init(GPIOA, &spiPins); /* SCK */
    spiPins.GPIO_PinNumber = GPIO_PIN_6;
    GPIO_Init(GPIOA, &spiPins); /* MISO */
    spiPins.GPIO_PinNumber = GPIO_PIN_7;
    GPIO_Init(GPIOA, &spiPins); /* MOSI */

    GPIOA->AFR[0] &= ~((0x0FU << 20U) | (0x0FU << 24U) | (0x0FU << 28U));
    GPIOA->AFR[0] |= ((5U << 20U) | (5U << 24U) | (5U << 28U));

    /* PA4 Pinini Yazılımsal CS (Chip Select) Olarak Ayarla (GPIO Output) */
    GPIO_PinConfig_t csPin = {
        .GPIO_PinNumber = GPIO_PIN_4,
        .GPIO_PinMode = GPIO_MODE_OUT,
        .GPIO_PinSpeed = GPIO_SPEED_HIGH,
        .GPIO_PinOPType = GPIO_OP_TYPE_PP,
        .GPIO_PinPuPdControl = GPIO_NO_PUPD
    };
    GPIO_Init(GPIOA, &csPin);
    GPIOA->BSRR = (1U << GPIO_PIN_4); /* CS High (Idle) */

    /* SPI1 Konfigürasyonu (Master Mode, Mode 3: CPOL=1, CPHA=1, Prescaler=/16) */
    SPI_Config_t spiConfig = {
        .SPI_DeviceMode = SPI_DEVICE_MODE_MASTER,
        .SPI_CPOL = SPI_CPOL_HIGH,
        .SPI_CPHA = SPI_CPHA_HIGH,
        .SPI_Speed = SPI_SPEED_DIV_16,
        .SPI_DFF = 0U /* 8-Bit Data */
    };
    SPI_Init(SPI1, &spiConfig);

    /* 4. I2C1 Pinleri (PB6=SCL, PB7=SDA) Alternatif Fonksiyona (AF4) ve Open-Drain'e Ayarla */
    GPIO_PinConfig_t i2cPins = {
        .GPIO_PinNumber = GPIO_PIN_6,
        .GPIO_PinMode = GPIO_MODE_ALTFN,
        .GPIO_PinSpeed = GPIO_SPEED_HIGH,
        .GPIO_PinOPType = GPIO_OP_TYPE_OD,  /* Open-Drain Zorunlu! */
        .GPIO_PinPuPdControl = GPIO_PIN_PU, /* Dahili Pull-Up Aktif */
        .GPIO_PinAltFunMode = 4U            /* AF4 = I2C1 */
    };
    GPIO_Init(GPIOB, &i2cPins); /* SCL */
    i2cPins.GPIO_PinNumber = GPIO_PIN_7;
    GPIO_Init(GPIOB, &i2cPins); /* SDA */

    GPIOB->AFR[0] &= ~((0x0FU << 24U) | (0x0FU << 28U));
    GPIOB->AFR[0] |= ((4U << 24U) | (4U << 28U));

    /* I2C1 Konfigürasyonu (100 kHz Standard Mode, 16 MHz APB1 Clock) */
    I2C_Config_t i2cConfig = {
        .I2C_SCLSpeed = I2C_SCL_SPEED_SM,
        .I2C_AckControl = 1U
    };
    I2C_Init(I2C1, &i2cConfig, 16000000U);

    /* 5. IMU WHO_AM_I Register'ını Oku (0x75) */
    uint8_t whoAmI = SPI_ReadRegister(SPI1, GPIOA, GPIO_PIN_4, 0x75U);

    /* 6. BMP280 Sensörünün Chip ID'sini Oku (Slave Address: 0x76, Reg: 0xD0) */
    uint8_t chipId = 0U;
    I2C_Status_t i2cStatus = I2C_ReadRegister(I2C1, 0x76U, 0xD0U, &chipId, 10000U);

    uint16_t systemStatus = 0x0000U;
    if (whoAmI == 0x70U) {
        systemStatus |= 0x0001U; /* SPI IMU Başarıyla Tespit Edildi */
    }
    if ((i2cStatus == I2C_STATUS_OK) && (chipId == 0x58U)) {
        systemStatus |= 0x0002U; /* I2C Sensörü Başarıyla Tespit Edildi */
    }

    TelemetryPacket_t pkt;
    uint32_t counter = 0U;

    while (1) {
        pkt.header = TELEMETRY_HEADER;
        pkt.packet_id = counter++;
        pkt.status_flags = systemStatus;

        if (systemStatus & 0x0002U) {
            /* I2C üzerinden okunan Chip ID değerini gönder (Test amaçlı) */
            pkt.temp_raw = (int16_t)chipId;
        } else {
            pkt.temp_raw = -1; /* Hata durumu */
        }

        /* CRC16 Hesapla (Header'dan status_flags'e kadar olan 10 Bayt) */
        pkt.crc16 = CRC16_Calculate((uint8_t *)&pkt, sizeof(TelemetryPacket_t) - sizeof(uint16_t));

        /* Telemetri Paketini UART Üzerinden Gönder */
        USART_SendData(USART2, (uint8_t *)&pkt, sizeof(TelemetryPacket_t));

        /* 1 Saniye Bekle */
        for (volatile uint32_t i = 0; i < 1000000; i++)
            ;
    }

    return 0;
}