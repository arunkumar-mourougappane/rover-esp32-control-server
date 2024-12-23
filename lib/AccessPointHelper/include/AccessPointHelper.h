/**
 * @file AccessPointHelper.h
 * @author Arunkumar Mourougappane (amouroug@buffalo.edu)
 * @brief Header containing prototype for Wrapper Class for Wifi AccessPoint.
 * @version 0.1
 * @date 2024-12-22
 * 
 * Copyright (c) Arunkumar Mourougappane
 * 
 */

#ifndef ACCESS_POINT_HELPER_H
#define ACCESS_POINT_HELPER_H
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiAP.h>

class CAccessPointHelper : public WiFiClass {
   public:
      CAccessPointHelper(String accesPointSSID, String accessPointPass);
      bool SetupAccessPoint();
      IPAddress GetAccessPointIP();
   private:
      String m_AccessPointSSID;
      String m_AccessPointPass;
};

#endif // !ACCESS_POINT_HELPER_H
