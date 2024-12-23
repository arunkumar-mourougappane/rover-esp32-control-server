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

/**
 * @brief A wrapper around Adafruit NeoPixel to
 * simply NeoPixel use on single pixel block Neo Pixel
 * hardware.
 * 
 */
class CNeoPixel : public Adafruit_NeoPixel
{
public:
   /**
    * @brief Construct a new CNeoPixel object
    * 
    * @param pixel_pin GPIO Pin to Neo Pixel Data
    * @param pixel_power_pin GPIO Pin to control Neo Pixel Power
    */
   CNeoPixel(uint8_t pixel_pin, uint8_t pixel_power_pin);
   /**
    * @brief Set the Pixel Color (Overwrites current RGB Setting).
    * 
    * @param color - 32 bit RGB representation.
    */
   void SetPixelColor(uint32_t color);
   /**
    * @brief Sets the new Pixel Color without overriding previous RGB Setting.
    * 
    * @param color - 32-bit RGB for the color to set.
    * @param clearOld - boolean flag to see if old data needs to be cleared or preserved.
    * @return uint32_t - Updated Pixel color after setting the pixel.
    */
   uint32_t UpdatePixelColor(uint32_t color, bool clearOld=false);
private:
   uint8_t m_PixelPowerPin;
};

#endif // !NEO_PIXEL_H