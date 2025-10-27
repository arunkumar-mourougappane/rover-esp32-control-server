/**
 * @file GrpcServer.h
 * @author Arunkumar Mourougappane (amouroug@buffalo.edu)
 * @brief gRPC server implementation for ESP32 rover control
 * @version 1.0.0
 * @date 2025-10-26
 * 
 * Copyright (c) Arunkumar Mourougappane
 * 
 */

#ifndef GRPC_SERVER_H
#define GRPC_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include "SensorData.h"
#include "JoystickData.h"
#include <AccessPointHelper.h>

// Since full gRPC is complex for ESP32, we'll implement a simplified
// protocol that mimics gRPC behavior but uses a lighter TCP-based approach
class CGrpcServer {
public:
    /**
     * @brief Construct a new CGrpcServer object
     * 
     * @param port Server port (default 50051 for gRPC)
     * @param SSID WiFi Access Point SSID
     * @param password WiFi Access Point password
     */
    CGrpcServer(int port, String SSID, String password);
    
    /**
     * @brief Set up and bring up Access Point Network
     */
    void SetupNetwork();
    
    /**
     * @brief Start the gRPC server
     */
    void StartServer();
    
    /**
     * @brief Handle incoming client connections and requests
     */
    void HandleClients();
    
    /**
     * @brief Updates the member variable instance of IMU Sensor data
     * 
     * @param imu_data An instance of IMU Sensor data
     */
    void UpdateImuData(imu_data_t imu_data);
    
    /**
     * @brief Get the latest joystick data received from client
     * 
     * @return joystick_data_t Latest joystick control data
     */
    joystick_data_t GetJoystickData();

private:
    /**
     * @brief Process incoming gRPC-like request
     * 
     * @param client WiFi client connection
     * @param request Raw request data
     */
    void ProcessRequest(WiFiClient& client, String request);
    
    /**
     * @brief Handle LED control requests
     * 
     * @param client WiFi client connection
     * @param ledOn true to turn LED on, false to turn off
     */
    void HandleLedControl(WiFiClient& client, bool ledOn);
    
    /**
     * @brief Handle IMU data requests
     * 
     * @param client WiFi client connection
     * @param specific_param Specific parameter to return (empty for all data)
     */
    void HandleImuDataRequest(WiFiClient& client, String specific_param = "");
    
    /**
     * @brief Handle joystick data from client
     * 
     * @param client WiFi client connection
     * @param joystick_json JSON string containing joystick data
     */
    void HandleJoystickData(WiFiClient& client, String joystick_json);
    
    /**
     * @brief Handle streaming IMU data request
     * 
     * @param client WiFi client connection
     * @param params Streaming parameters (rate, duration, etc.)
     */
    void HandleStreamImuData(WiFiClient& client, String params);
    
    /**
     * @brief Send response in gRPC-like format
     * 
     * @param client WiFi client connection
     * @param response Response data
     */
    void SendResponse(WiFiClient& client, String response);
    
    /**
     * @brief Send streaming data packet
     * 
     * @param client WiFi client connection
     * @param data Data to stream
     * @param isLast True if this is the last packet in stream
     */
    void SendStreamData(WiFiClient& client, String data, bool isLast = false);

    // Server configuration
    int m_Port;
    WiFiServer m_Server;
    CAccessPointHelper m_AccessPoint;
    
    // Local instance data of IMU Sensor output
    imu_data_t m_ImuData;
    
    // Local instance data of Joystick control input
    joystick_data_t m_JoystickData;
    
    // Server running state
    bool m_ServerRunning;
    
    // Streaming state
    WiFiClient m_StreamingClient;
    bool m_IsStreaming;
    unsigned long m_LastStreamTime;
    unsigned int m_StreamingRate;  // Streaming rate in Hz
};

#endif // !GRPC_SERVER_H