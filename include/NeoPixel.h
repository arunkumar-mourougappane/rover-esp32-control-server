/**
 * @file NeoPixel.h
 * @author Arunkumar Mourougappane (amouroug@buffalo.edu)
 * @brief A class instance to help handle NeoPixel H/W.
 * @version 0.1
 * @date 2024-12-22
 *
 * Copyright (c) Arunkumar Mourougappane
 *
 */

#ifndef NEO_PIXEL_H
#define NEO_PIXEL_H
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#define MAX_PIXEL_COUNT 1

class CNeoPixel : public Adafruit_NeoPixel
{
public:
   CNeoPixel(uint8_t pixel_pin, uint8_t pixel_power_pin);
   static void ScopedSetNeoPixel(uint8_t pixelPin, uint8_t pixelPowerPin, neoPixelType pixelType, uint32_t pixelColor );
   void SetPixelColor(uint32_t color);
private:
   uint8_t m_PixelPowerPin;
};

#endif // !NEO_PIXEL_H