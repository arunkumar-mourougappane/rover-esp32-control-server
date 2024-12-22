/**
 * @file CNeoPixel.cpp
 * @author Arunkumar Mourougappane (amouroug@buffalo.edu)
 * @brief 
 * @version 0.1
 * @date 2024-12-22
 * 
 * Copyright (c) Arunkumar Mourougappane
 * 
 */
#include <mutex>
#include "NeoPixel.h"
#include <Arduino.h>


CNeoPixel::CNeoPixel(uint8_t pixel_pin, uint8_t pixel_power_pin) : Adafruit_NeoPixel(MAX_PIXEL_COUNT, pixel_pin, NEO_GRB + NEO_KHZ800), m_PixelPowerPin(pixel_power_pin) {
   pinMode(m_PixelPowerPin, OUTPUT);
   digitalWrite(m_PixelPowerPin, HIGH);
   begin();
   m_Initialized = true;
}

void CNeoPixel::SetPixelColor(uint32_t color) {
   setPixelColor(MAX_PIXEL_COUNT-1, color);
   show();
}
