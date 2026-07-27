#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <stdint.h>

#define TELEMETRY_HEADER 0xA55AU

/* Hizasız/Padding Olmayan Paket Yapısı (Tam 12 Byte) */
typedef struct __attribute__((packed)) {
  uint16_t header;       /* 2 Byte: 0xA55A */
  uint32_t packet_id;    /* 4 Byte: Paket Sayacı */
  int16_t temp_raw;      /* 2 Byte: Sıcaklık Sensör Verisi */
  uint16_t status_flags; /* 2 Byte: Sistem Durum Bayrakları */
  uint16_t crc16;        /* 2 Byte: CRC-16 Checksum (Header-Status arası) */
} TelemetryPacket_t;

#endif /* TELEMETRY_H */