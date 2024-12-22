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
}

void CNeoPixel::SetPixelColor(uint32_t color) {
   if (canShow() == true) {
      setPixelColor(MAX_PIXEL_COUNT-1, color);
      show();
   }
}

uint32_t CNeoPixel::UpdatePixelColor(uint32_t color, bool clearOld) {
   uint32_t lastColor = getPixelColor(MAX_PIXEL_COUNT-1);
   uint8_t lastColorRed = (lastColor >> 16) & 0xFF;
   uint8_t lastColorGreen = (lastColor >> 8) & 0xFF;
   uint8_t lastColorBlue = (lastColor) & 0xFF;
   /*
      if clearOld is true, we do no retain the old values and simply overwrite the pixel colors
      and, if its false, we get the old values and bitwise OR to retain old data.
   */
   uint8_t newRed = (clearOld == true)?((color >> 16) & 0xFF):(lastColorRed|((color >> 16) & 0xFF));

   uint8_t newGreen = (clearOld == true)?((color >> 8) & 0xFF):(lastColorGreen|((color >> 8) & 0xFF));

   uint8_t newBlue = (clearOld == true)?(color & 0xFF):(lastColorBlue|(color & 0xFF));

   uint32_t newColor = Color(newRed, newGreen, newBlue);
   if (canShow() == true) {
      setPixelColor(MAX_PIXEL_COUNT-1, newColor);
      show();
      return newColor;
   }
   return 0;
}
