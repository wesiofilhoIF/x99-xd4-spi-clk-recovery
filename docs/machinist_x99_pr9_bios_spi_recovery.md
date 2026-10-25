---
title: "Machinist X99-PR9 — Localização do Chip BIOS, Mapeamento de Trilhas e Recuperação SPI"
date: "2026-10-25"
type: "hardware-guide"
system: "Machinist X99-PR9"
hardware: ["Machinist X99-PR9", "Intel Q85/Q87/C612", "W25Q128JV"]
target: ["RomEMU", "ESP32-S3", "ESP32-P4"]
status: "active"
tags:
  - "homelab"
  - "bios"
  - "machinist"
  - "pr9"
  - "spi"
  - "recuperacao"
---

# Machinist X99-PR9 — Localização do Chip BIOS, Mapeamento de Trilhas e Recuperação SPI

**Data:** 25/10/2026  
**Placa-Mãe:** Machinist X99-PR9 (LGA 2011-3, Southbridge Intel Q85/Q87/C612)  
**Objetivo:** Localização física do chip de BIOS, identificação da matriz de resistores de amortecimento SPI e runbook de recuperação de pads arrancados (CLK Pino 6 / MOSI Pino 5) para emulação via RomEMU (ESP32-S3).

---

## 1. Localização Física Exata no PCB da Machinist X99-PR9

Diferente da Jieshuo X99-XD4 (onde a BIOS fica espremida entre conectores SATA e headers de painel), a **Machinist X99-PR9** possui um layout muito mais espaçoso e acessível para solda de bancada.

```
┌─────────────────────────────────────────────────────────────────┐
│                    TOPOLOGIA DA MACHINIST X99-PR9               │
│                                                                 │
│  [Painel Traseiro I/O]     [Soquete LGA 2011-3]     [4x DDR4]   │
│                                                                 │
│  [Slot PCIe 3.0 x16] ───────────────────────────────────────── │
│                                                                 │
│  [Bateria CR2032]   [Super I/O]    ┌──────────┐  [CE45]         │
│                                    │DISSIPADOR│  [Resistores]   │
│  [PCIe x1]                         │PRETO PCH │  ★ BIOS SOP8 ★  │
│                                    │ (Intel)  │  [Cristal RTC]  │
│                                    └──────────┘  [CE92] [SATA]  │
└─────────────────────────────────────────────────────────────────┘
```

### Pontos de Referência Visuais:
1. **Posicionamento:** Canto inferior direito do PCB, **imediatamente à direita do dissipador preto com aletas verticais do chipset PCH**.
2. **Superior:** Capacitor eletrolítico sólido `CE45` ($820\,\mu\text{F} / 2.5\text{V}$).
3. **Inferior:** Cristal oscilador cilíndrico prateado do RTC ($32.768\text{ kHz}$) com alça de terra soldada ao plano de cobre, e capacitor `CE92` ($100\,\mu\text{F} / 6.3\text{V}$).
4. **Direita:** CI SMD menor `U24` (amplificador operacional duplo LM358).
5. **Esquerda:** Corredor vertical de resistores de terminação e amortecimento colado ao dissipador do PCH.

---

## 2. Anatomia do Circuito SPI na PR9: A Rede de Resistores (Caixa Amarela)

O circuito SPI Flash na Machinist PR9 adota redes de resistores SMD em série entre o chipset e o chip Winbond W25Q128JV (SOP8):

* **Caixa Verde (Chip BIOS):**
  * Encapsulamento SOP8 (8 pinos, 4 em cima e 4 embaixo).
  * Pino 1 (`CS#`): Canto com chanfro/ponto guia.
  * Pino 2 (`DO / MISO`): Saída de dados para a placa.
  * Pino 5 (`DI / MOSI`): Entrada de comandos do PCH.
  * Pino 6 (`CLK`): Entrada de clock mestre do PCH.
* **Caixa Amarela (Corredor de Resistores entre PCH e BIOS):**
  * Conjunto vertical de redes resistivas SMD (Dual/Quad packs ou resistores 0402 individuais).
  * **Lado Esquerdo dos Resistores:** Conectado diretamente às trilhas internas das camadas do PCB que vêm do chipset Intel Southbridge.
  * **Lado Direito dos Resistores:** Conectado diretamente aos pads das 8 pernas do chip da BIOS.

---

## 3. Metodologia de Recuperação para RomEMU (Procedimento em 3 Etapas)

Se algum pad do chip da BIOS foi arrancado na Machinist PR9 durante desoldagem (comum nos pinos 5 ou 6 devido à rigidez de fios AWG 24):

### Etapa 1: O Teste do Pino Guia (Sweep de Continuidade)
1. Colocar o multímetro na escala de continuidade (bip).
2. Fixar uma ponta de prova em um pino **íntegro** da BIOS:
   * **Pino 1 (`CS#`)** ou **Pino 2 (`MISO`)**.
3. Varrer o lado direito dos resistores da **Caixa Amarela**:
   * O multímetro apitará em $0\,\Omega$ exatamente no resistor que amortece aquela linha.
   * Identifica-se assim o padrão de valor nominal (geralmente $15\,\Omega$, $22\,\Omega$ ou $33\,\Omega$).

### Etapa 2: Identificação do Resistor Órfão
1. Na mesma fileira/bloco, medir os resistores vizinhos de valor idêntico.
2. O resistor que **não apresenta continuidade com nenhuma perna física da BIOS** é o **resistor órfão** do pino cujo pad foi rompido no PCB.
3. Teste confirmatório com multímetro na escala de diodo (ponta vermelha no GND da carcaça USB, ponta preta no lado esquerdo do resistor órfão):
   * Queda de tensão reversa entre $0.40\text{ V}$ e $0.65\text{ V}$ confirma a junção PN interna do chipset Intel (sinal vivo!).

### Etapa 3: Ponto de Solda do Jumper (Fio AWG 30)
1. Aplicar micro-gota de fluxo no terminal do resistor órfão (lado voltado para o chipset PCH).
2. Soldar a ponta desencapada de um fio esmaltado fino (AWG 30 / fiapo de cobre flexível).
3. Conectar a outra ponta diretamente no conector do **ESP32-S3** rodando o firmware **RomEMU**:

| Sinal SPI | Pino BIOS (SOP8) | Ponto de Solda na PR9 | Pino ESP32-S3 | Função no RomEMU |
|:---|:---:|:---|:---:|:---|
| **CS#** | Pino 1 | Resistor de amortecimento do Pino 1 | **GPIO 10 (`IO10`)** | Chip Select mestre |
| **MOSI / DI** | Pino 5 | Resistor órfão do Pino 5 | **GPIO 11 (`IO11`)** | Comandos PCH -> ROM |
| **CLK** | Pino 6 | Resistor órfão do Pino 6 | **GPIO 12 (`IO12`)** | **Clock Mestre Obrigatório** |
| **MISO / DO** | Pino 2 | Resistor de amortecimento do Pino 2 | **GPIO 13 (`IO13`)** | Dados ROM -> PCH |
| **GND** | Pino 4 | Carcaça metálica USB / Plano ATX | **GND Comum** | Referência 0V de sinal |

---

## 4. Vantagens da Machinist PR9 vs Jieshuo X99-XD4
1. **Acessibilidade:** Na PR9, os resistores estão totalmente expostos entre o dissipador e a BIOS, sem proximidade de botões de liga/desliga ou conectores SATA bloqueando o ferro de solda.
2. **Espaçamento:** Maior área de cobre entre os componentes SMD, reduzindo drasticamente o risco de pontes de estanho acidentais.
3. **Reputação de Hardware:** A Machinist PR9 utiliza chipset Intel Q85/Q87/C612 com compatibilidade total ao emulador SPI do RomEMU em frequências de 20 MHz (Single SPI / Fast Read `0Bh`).
