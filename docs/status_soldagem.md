# Status de Soldagem de Bancada (X99-XD4 <-> ESP32-S3 RomEMU)

Data: 24/10/2026

## Pinos da BIOS (Winbond W25Q128 / SOP-8):
- **Pino 1:** CS# (Chip Select)
- **Pino 2:** DO / MISO (Data Out da BIOS) — Pad INTACTO
- **Pino 3:** WP# (Write Protect)
- **Pino 4:** GND (Terra) — Pad INTACTO
- **Pino 5:** DI / MOSI (Data In da BIOS) — **PAD FOI ARRANCADO**
- **Pino 6:** CLK (SPI Clock) — **PAD FOI ARRANCADO**
- **Pino 7:** HOLD# (Pause)
- **Pino 8:** VCC (3.3V) — Não conectar

## Estado Atual da Bancada:
1. **Pino 1 (CS#):** Soldado no Pad 1 intacto -> **IO10** do ESP32-S3
2. **Pino 2 (DO / MISO):** Soldado no Pad 2 intacto -> **IO13** do ESP32-S3
3. **Pino 6 (CLK):** Soldado no Resistor 3 (Jumper 1) -> **IO12** do ESP32-S3

## O Que Falta Agora:
4. **Pino 5 (DI / MOSI):** Soldar no **Resistor 2 (Jumper 2 - Verde)** -> **IO11** do ESP32-S3
5. **Pino 4 (GND):** Soldar no **Pad 4 intacto** -> **GND** do ESP32-S3
