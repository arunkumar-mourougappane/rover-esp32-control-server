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
 * @brief  Pins
 */
#define LSM6DOX_SDA_PIN 42
#define LSM6DOX_SCL_PIN 41

/**
 * @brief NeoPixel Pins
 *
 */
#define NEOPIXEL_DATA_PIN 33
#define NEOPIXEL_POWER_PIN 34
/**
 * @brief NeoPixel Count.
 *
 */
#define NEOPIXEL_COUNT 1

/**
 * @brief Task instantiations.
 *
 */
TaskHandle_t sensor_process_task;
TaskHandle_t web_handler_task;


void Task0code(void *);
void Task1code(void *);

/**
 * @brief AccessPoint Credentials
 * 
 */

const String ROVER_AP_SSID = String("MOONBASE-II");
const String ROVER_AP_PASS_PHRASE = String("Trypt1c0n$");

CNeoPixel pixels(NEOPIXEL_DATA_PIN, NEOPIXEL_DATA_PIN);
CAccessPointHelper roverNetwork(ROVER_AP_SSID, ROVER_AP_PASS_PHRASE);
#endif // !ROVER_SERVER_H