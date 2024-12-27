/**
 * @file SensorData.h
 * @author Arunkumar Mourougappane (amouroug@buffalo.edu)
 * @brief Header defining the structure for each sensor data.
 * @version 0.1
 * @date 2024-12-26
 * 
 * Copyright (c) Arunkumar Mourougappane
 * 
 */

#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

typedef struct {
   float accX;
   float accY;
   float accZ;
   float gyroX;
   float gyroY;
   float gyroZ;
   float temperature;
} imu_data_t;

#endif // !SENSOR_DATA_H
