/**
 * @file AccessPointHelper.cpp
 * @author Arunkumar Mourougappane (amouroug@buffalo.edu)
 * @brief Implementation of Accesspoint Helper class.
 * @version 0.1
 * @date 2024-12-22
 * 
 * Copyright (c) Arunkumar Mourougappane
 * 
 */

#include "AccessPointHelper.h"

CAccessPointHelper::CAccessPointHelper(String accesPointSSID, String accessPointPass) : WiFiClass(), m_AccessPointSSID(accesPointSSID), m_AccessPointPass(accessPointPass) {
   Serial.printf("WiFi AP Instantized.");
}

bool CAccessPointHelper::SetupAccessPoint() {
   if(softAP(m_AccessPointSSID, m_AccessPointPass) == true) {
      log_i("Accesspoint %s has been setup successfully.", m_AccessPointSSID.c_str());
      return true;
   }
   log_e("Soft AP creation failed.");
   return false;
}

IPAddress CAccessPointHelper::GetAccessPointIP() {
   return WiFi.softAPIP();
}