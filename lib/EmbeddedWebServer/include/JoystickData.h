/**
 * @file JoystickData.h
 * @author Arunkumar Mourougappane (amouroug@buffalo.edu)
 * @brief Header defining the structure for joystick control data.
 * @version 1.0.0
 * @date 2025-10-26
 * 
 * Copyright (c) Arunkumar Mourougappane
 * 
 */

#ifndef JOYSTICK_DATA_H
#define JOYSTICK_DATA_H

typedef struct {
   // Left joystick analog values (0-4095 for 12-bit ADC)
   int left_x;      // Left joystick X axis
   int left_y;      // Left joystick Y axis
   
   // Right joystick analog values (0-4095 for 12-bit ADC)
   int right_x;     // Right joystick X axis  
   int right_y;     // Right joystick Y axis
   
   // Optional button states (if joysticks have push buttons)
   bool left_button;  // Left joystick button pressed
   bool right_button; // Right joystick button pressed
   
   // Metadata
   unsigned long timestamp; // Timestamp when data was captured
} joystick_data_t;

#endif // !JOYSTICK_DATA_H