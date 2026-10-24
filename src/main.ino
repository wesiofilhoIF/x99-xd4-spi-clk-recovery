/*
 * Testador de Diodo vs Resistor (Bipolaridade)
 * Placa: ESP32-C6
 * 
 * Se for DIODO: Conduz em um sentido e bloqueia no outro (unidirecional).
 * Se for RESISTOR: Conduz identicamente nos dois sentidos (bidirecional).
 */

#include <Arduino.h>

#define RGB_PIN 8
#define ADC_PIN 0

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  pinMode(ADC_PIN, INPUT);
}

void loop() {
  int raw = analogRead(ADC_PIN);
  float v = (raw / 4095.0) * 3.3;

  if (v > 2.8) {
    rgbLedWrite(RGB_PIN, 0, 255, 0); // Verde (3.3V)
  } else if (v > 0.3) {
    rgbLedWrite(RGB_PIN, 255, 120, 0); // Laranja (PCH)
  } else {
    rgbLedWrite(RGB_PIN, 0, 15, 70); // Azul (0V)
  }
  delay(20);
}
