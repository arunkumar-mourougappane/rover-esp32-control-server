#include <Arduino.h>

int led_bit = HIGH;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, led_bit);
  led_bit = !led_bit;
  delay(500);
}

