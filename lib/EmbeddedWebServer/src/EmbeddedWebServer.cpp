/**
 * @file EmbeddedWebServer.cpp
 * @author Arunkumar Mourougappane (amouroug@buffalo.edu)
 * @brief An implementation for CEmbeddedWebServer member functions.
 * @version 0.1
 * @date 2024-12-26
 *
 * Copyright (c) Arunkumar Mourougappane
 *
 */
#include "EmbeddedWebServer.h"
#include <ArduinoJson.h>

CEmbeddedWebServer::CEmbeddedWebServer(int port, String SSID, String password) : WebServer(80), m_AccessPoint(SSID, password)
{
   memset(&m_ImuData, 0, sizeof(imu_data_t));
}

void CEmbeddedWebServer::SetupNetwork()
{
   // Wait till Access Point is setup.
   while (!m_AccessPoint.SetupAccessPoint())
   {
      log_e("AP Setup Failed. Waiting to retry...");
      delay(100);
   }
   
   log_i("AP IP Address: %s\n", m_AccessPoint.GetAccessPointIP().toString().c_str());
}

void CEmbeddedWebServer::SetUpWebHandlers()
{
   // Setup web handle for root www.
   on("/", [this]()
      { this->handleRoot(); });
   // Setup web handle for HTTP 404 Error.
   onNotFound([this]()
              { this->handleNotFound(); });
   // Setup web handle for turning LED ON.
   on("/led-on", [this]()
      { this->turnBuildInLEDOn(); });
   // Setup web handle for turning LED OFF.
   on("/led-off", [this]()
      { this->turnBuildInLEDOff(); });
   // Setup web handle for getting JSON version of IMU Data.
   on("/all-imu-data", [this]()
      { this->getIMUData(); });
   // Setup web handle for getting JSON version of IMU Data.
   on("/specific-imu-data", [this]()
      { this->getIMUDataOnRequest(); });
   begin();
}

void CEmbeddedWebServer::updateImuData(imu_data_t imuData)
{
   // Make a local copy from the provided data.
   memcpy(&m_ImuData, &imuData, sizeof(imu_data_t));
}

void CEmbeddedWebServer::handleNotFound()
{
   // Return 404 Error
   send(404, "text/plain", "File Not Found");
}

void CEmbeddedWebServer::handleRoot()
{
   // Return HTTP 200 Error
   send(200, "text/plain", "hello from esp32!");
}

void CEmbeddedWebServer::turnBuildInLEDOff()
{
   digitalWrite(BUILTIN_LED, LOW);
   log_i("Led Toggled OFF");
   send(200, "text/plain", "Ok");
}

void CEmbeddedWebServer::turnBuildInLEDOn()
{
   digitalWrite(BUILTIN_LED, HIGH);
   log_i("Led Toggled ON");
   send(200, "text/plain", "Ok");
}

void CEmbeddedWebServer::getIMUData()
{
   // Set JSON object
   JsonDocument doc;
   JsonObject root = doc.to<JsonObject>();
   // Create a JSON Dictionary
   root["accX"] = m_ImuData.accX;
   root["accY"] = m_ImuData.accY;
   root["accZ"] = m_ImuData.accZ;
   root["gyroX"] = m_ImuData.gyroX;
   root["gyroY"] = m_ImuData.gyroY;
   root["gyroZ"] = m_ImuData.gyroZ;
   root["temperature"] = m_ImuData.temperature;
   // Serialize to String
   String imuDataString;
   serializeJson(doc, imuDataString);
   log_d("Serialized Data: %s", imuDataString.c_str());
   // Send back response.
   send(200, "application/json", imuDataString.c_str());
}

void CEmbeddedWebServer::getIMUDataOnRequest()
{
   // Set JSON object
   JsonDocument doc;
   JsonObject root = doc.to<JsonObject>();
   String parameter = this->arg("parameter");

   if (parameter.length() >0)
   {
      Serial.print("Received GET request with parameters: ");
      JsonDocument doc;
      JsonObject root = doc.to<JsonObject>();
      if (parameter.compareTo(String("acc")) == 0) {
         root["accX"] = m_ImuData.accX;
         root["accY"] = m_ImuData.accY;
         root["accZ"] = m_ImuData.accZ;
      }
      else if (parameter.compareTo(String("gyro")) == 0) {
         root["gyroX"] = m_ImuData.gyroX;
         root["gyroY"] = m_ImuData.gyroY;
         root["gyroZ"] = m_ImuData.gyroY;
      }
      else if (parameter.compareTo(String("accx")) == 0) {
         root["accX"] = m_ImuData.accX;
      }
      else if (parameter.compareTo(String("accy")) == 0) {
         root["accY"] = m_ImuData.accY;
      }
      else if (parameter.compareTo(String("accz")) == 0) {
         root["accZ"] = m_ImuData.accZ;
      }
      else if (parameter.compareTo(String("gyrox")) == 0) {
         root["gyroX"] = m_ImuData.gyroX;
      }
      else if (parameter.compareTo(String("gyroy")) == 0) {
         root["gyroY"] = m_ImuData.gyroY;
      }
      else if (parameter.compareTo(String("gyroz")) == 0) {
         root["gyroZ"] = m_ImuData.gyroZ;
      }
      else if (parameter.compareTo(String("temperature")) == 0) {
         root["temperature"] = m_ImuData.temperature;
      }
      else {
         send(400, "text/plain", "Bad Request");
         log_e("Incoming request is unsupported.");
         return;
      }
      // Serialize to String
      String imuDataString;
      serializeJson(doc, imuDataString);
      send(200, "text/plain", imuDataString);
   }
   else
   {
      log_e("Incoming request is unsupported.");
      send(400, "text/plain", "Bad Request");
   }
}
