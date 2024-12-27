/**
 * @file EmbeddedServer.h
 * @author Arunkumar Mourougappane (amouroug@buffalo.edu)
 * @brief A wrapper library for arduino embedded web server to handle
 *        commands and to provide sensor information.
 * @version 0.1
 * @date 2024-12-25
 * 
 * Copyright (c) Arunkumar Mourougappane
 * 
 */
#ifndef EMBEDDED_SERVER_H
#define EMBEDDED_SERVER_H

#include <Arduino.h>
#include <WebServer.h>
#include "SensorData.h"
#include <AccessPointHelper.h>

#define ACCELERATION_X "acc_x"
#define ACCELERATION_Y "acc_y"
#define ACCELERATION_Z "acc_z"
#define GYROSCOPE_X    "gyro_x"
#define GYROSCOPE_Y    "gyro_y"
#define GYROSCOPE_Z    "gyro_z"

class CEmbeddedWebServer : public WebServer {
   public:
      /**
       * @brief Construct a new CEmbeddedWebServer object
       * 
       * @param port Web server port number 80.
       * @param SSID SSID for the wifi Access point
       * @param password password fpr the wifi access point
       */
      CEmbeddedWebServer(int port, String SSID, String password);
      /**
       * @brief Set up and bring up Access Point Network.
       * 
       */
      void SetupNetwork();
      /**
       * @brief A member function setting up web handlers for HTTP GET and POST calls.
       * 
       */
      void SetUpWebHandlers();
      /**
       * @brief A member routine for handling requests.
       * 
       */
      void handleRequest();
      /**
       * @brief Updates the member variable instance of IMU Sensor data.
       * 
       * @param imu_data An instance of IMU Sensor data.
       */
      void updateImuData(imu_data_t imu_data);
   private:
      /**
       * @brief A private member function to setup handle response for requests
       *        that return HTTP 404 Error code.
       * 
       */
      void handleNotFound();
      /**
       * @brief A private member function to handle root of web server.
       * 
       */
      void handleRoot();
      /**
       * @brief A private member function for the web handler to turn LED ON.
       * 
       */
      void turnBuildInLEDOn();
      /**
       * @brief A private member function for the web handler to turn LED OFF.
       * 
       */
      void turnBuildInLEDOff();
      /**
       * @brief A private member function for the web handler to handle commands.
       * 
       */
      void handleClientCommands();
      /**
       * @brief A web handle to return IMU Data from sensor.
       * 
       */
      void getIMUData();
      /**
       * @brief Web Handle to get specific imu data.
       * 
       */
      void getIMUDataOnRequest();
      // Access Point Information.
      CAccessPointHelper m_AccessPoint;
      // Local instance data of IMU Sensor ouput.
      imu_data_t m_ImuData;
};

#endif // !EMBEDDED_SERVER_H
