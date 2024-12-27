/**
 * @file rover_server.h
 * @author Arunkumar Mourougappane (amouroug@buffalo.edu)
 * @brief Header file containing prototypes and definitions for
 *        main task of the esp32.
 * @version 0.1
 * @date 2024-12-22
 * 
 * Copyright (c) Arunkumar Mourougappane
 * 
 */

#ifndef ROVER_SERVER_H
#define ROVER_SERVER_H
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_LSM6DSOX.h> // LSM6DS for 6-DOF Measurement
#include "NeoPixel.h"
#include "AccessPointHelper.h"

/**
 * @brief Task instantiations.
 *
 */
TaskHandle_t sensor_process_task;
TaskHandle_t web_handler_task;

/**
 * @brief Tasks for Reading Sensor data and passing to Web Server.
 * 
 */
void SensorDataTask(void *);
void WebServerTask(void *);
/**
 * @brief A queue to handle IMU Sensor Data.
 * 
 */
QueueHandle_t imuSensorQueue;

/**
 * @brief AccessPoint Credentials
 * 
 */
const String ROVER_AP_SSID = String("MOONBASE-II");
const String ROVER_AP_PASS_PHRASE = String("Trypt1c0n$");

#endif // !ROVER_SERVER_H