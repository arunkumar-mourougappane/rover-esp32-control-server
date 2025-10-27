/**
 * @file GrpcServer.cpp
 * @author Arunkumar Mourougappane (amouroug@buffalo.edu)
 * @brief Implementation of gRPC server for ESP32 rover control
 * @version 1.0.0
 * @date 2025-10-26
 *
 * Copyright (c) Arunkumar Mourougappane
 *
 */

#include "GrpcServer.h"
#include <ArduinoJson.h>

// gRPC-like message types
#define MSG_LED_ON "TurnLedOn"
#define MSG_LED_OFF "TurnLedOff"
#define MSG_GET_ALL_IMU "GetAllImuData"
#define MSG_GET_SPECIFIC_IMU "GetSpecificImuData"
#define MSG_SEND_JOYSTICK "SendJoystickData"
#define MSG_STREAM_IMU "StreamImuData"

CGrpcServer::CGrpcServer(int port, String SSID, String password) 
    : m_Port(port), m_Server(port), m_AccessPoint(SSID, password), m_ServerRunning(false),
      m_IsStreaming(false), m_LastStreamTime(0), m_StreamingRate(10)
{
    memset(&m_ImuData, 0, sizeof(imu_data_t));
    memset(&m_JoystickData, 0, sizeof(joystick_data_t));
}

void CGrpcServer::SetupNetwork()
{
    // Wait till Access Point is setup
    while (!m_AccessPoint.SetupAccessPoint())
    {
        log_e("AP Setup Failed. Waiting to retry...");
        delay(100);
    }
    
    log_i("AP IP Address: %s", m_AccessPoint.GetAccessPointIP().toString().c_str());
}

void CGrpcServer::StartServer()
{
    m_Server.begin();
    m_ServerRunning = true;
    log_i("gRPC-like Server started on port %d", m_Port);
}

void CGrpcServer::HandleClients()
{
    if (!m_ServerRunning) return;
    
    // Handle streaming to existing client
    if (m_IsStreaming && m_StreamingClient.connected())
    {
        unsigned long currentTime = millis();
        unsigned long interval = 1000 / m_StreamingRate; // Convert Hz to ms interval
        
        if (currentTime - m_LastStreamTime >= interval)
        {
            // Send IMU data to streaming client
            JsonDocument doc;
            doc["acc_x"] = m_ImuData.accX;
            doc["acc_y"] = m_ImuData.accY;
            doc["acc_z"] = m_ImuData.accZ;
            doc["gyro_x"] = m_ImuData.gyroX;
            doc["gyro_y"] = m_ImuData.gyroY;
            doc["gyro_z"] = m_ImuData.gyroZ;
            doc["temperature"] = m_ImuData.temperature;
            doc["timestamp"] = currentTime;
            doc["success"] = true;
            
            String streamData;
            serializeJson(doc, streamData);
            SendStreamData(m_StreamingClient, streamData);
            
            m_LastStreamTime = currentTime;
        }
    }
    else if (m_IsStreaming && !m_StreamingClient.connected())
    {
        // Streaming client disconnected
        log_i("Streaming client disconnected");
        m_IsStreaming = false;
    }
    
    // Handle new client connections
    WiFiClient client = m_Server.available();
    if (client)
    {
        log_i("New client connected");
        
        while (client.connected())
        {
            if (client.available())
            {
                String request = client.readStringUntil('\n');
                request.trim();
                
                if (request.length() > 0)
                {
                    log_d("Received request: %s", request.c_str());
                    ProcessRequest(client, request);
                }
            }
            
            // Small delay to prevent blocking
            delay(1);
        }
        
        client.stop();
        log_i("Client disconnected");
    }
}

void CGrpcServer::UpdateImuData(imu_data_t imuData)
{
    // Make a local copy from the provided data
    memcpy(&m_ImuData, &imuData, sizeof(imu_data_t));
}

void CGrpcServer::ProcessRequest(WiFiClient& client, String request)
{
    // Parse simple gRPC-like protocol: METHOD:PARAMS
    int colonIndex = request.indexOf(':');
    String method = request;
    String params = "";
    
    if (colonIndex > 0)
    {
        method = request.substring(0, colonIndex);
        params = request.substring(colonIndex + 1);
    }
    
    // Handle different RPC methods
    if (method == MSG_LED_ON)
    {
        HandleLedControl(client, true);
    }
    else if (method == MSG_LED_OFF)
    {
        HandleLedControl(client, false);
    }
    else if (method == MSG_GET_ALL_IMU)
    {
        HandleImuDataRequest(client);
    }
    else if (method == MSG_GET_SPECIFIC_IMU)
    {
        HandleImuDataRequest(client, params);
    }
    else if (method == MSG_SEND_JOYSTICK)
    {
        HandleJoystickData(client, params);
    }
    else if (method == MSG_STREAM_IMU)
    {
        HandleStreamImuData(client, params);
    }
    else
    {
        // Unknown method - send error response
        JsonDocument doc;
        doc["success"] = false;
        doc["error"] = "Unknown method: " + method;
        
        String response;
        serializeJson(doc, response);
        SendResponse(client, response);
    }
}

void CGrpcServer::HandleLedControl(WiFiClient& client, bool ledOn)
{
    // Control the built-in LED
    digitalWrite(BUILTIN_LED, ledOn ? HIGH : LOW);
    
    log_i("LED turned %s", ledOn ? "ON" : "OFF");
    
    // Send response
    JsonDocument doc;
    doc["success"] = true;
    doc["message"] = ledOn ? "LED turned ON" : "LED turned OFF";
    
    String response;
    serializeJson(doc, response);
    SendResponse(client, response);
}

void CGrpcServer::HandleImuDataRequest(WiFiClient& client, String specific_param)
{
    JsonDocument doc;
    
    if (specific_param.length() == 0)
    {
        // Return all IMU data
        doc["acc_x"] = m_ImuData.accX;
        doc["acc_y"] = m_ImuData.accY;
        doc["acc_z"] = m_ImuData.accZ;
        doc["gyro_x"] = m_ImuData.gyroX;
        doc["gyro_y"] = m_ImuData.gyroY;
        doc["gyro_z"] = m_ImuData.gyroZ;
        doc["temperature"] = m_ImuData.temperature;
        doc["timestamp"] = millis();
        doc["success"] = true;
    }
    else
    {
        // Return specific parameter
        doc["success"] = true;
        doc["timestamp"] = millis();
        
        if (specific_param == "acc")
        {
            doc["acc_x"] = m_ImuData.accX;
            doc["acc_y"] = m_ImuData.accY;
            doc["acc_z"] = m_ImuData.accZ;
        }
        else if (specific_param == "gyro")
        {
            doc["gyro_x"] = m_ImuData.gyroX;
            doc["gyro_y"] = m_ImuData.gyroY;
            doc["gyro_z"] = m_ImuData.gyroZ;
        }
        else if (specific_param == "accx")
        {
            doc["acc_x"] = m_ImuData.accX;
        }
        else if (specific_param == "accy")
        {
            doc["acc_y"] = m_ImuData.accY;
        }
        else if (specific_param == "accz")
        {
            doc["acc_z"] = m_ImuData.accZ;
        }
        else if (specific_param == "gyrox")
        {
            doc["gyro_x"] = m_ImuData.gyroX;
        }
        else if (specific_param == "gyroy")
        {
            doc["gyro_y"] = m_ImuData.gyroY;
        }
        else if (specific_param == "gyroz")
        {
            doc["gyro_z"] = m_ImuData.gyroZ;
        }
        else if (specific_param == "temperature")
        {
            doc["temperature"] = m_ImuData.temperature;
        }
        else
        {
            doc["success"] = false;
            doc["error"] = "Unknown parameter: " + specific_param;
        }
    }
    
    String response;
    serializeJson(doc, response);
    SendResponse(client, response);
    
    log_d("Sent IMU data response: %s", response.c_str());
}

void CGrpcServer::HandleJoystickData(WiFiClient& client, String joystick_json)
{
    if (joystick_json.length() == 0) {
        JsonDocument response_doc;
        response_doc["success"] = false;
        response_doc["message"] = "Empty joystick data";
        response_doc["timestamp"] = millis();
        
        String response;
        serializeJson(response_doc, response);
        SendResponse(client, response);
        return;
    }
    
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, joystick_json);
    
    if (error) {
        log_e("Joystick JSON parsing failed: %s", error.c_str());
        JsonDocument response_doc;
        response_doc["success"] = false;
        response_doc["message"] = "JSON parsing failed";
        response_doc["timestamp"] = millis();
        
        String response;
        serializeJson(response_doc, response);
        SendResponse(client, response);
        return;
    }
    
    // Parse joystick data from JSON
    m_JoystickData.left_x = doc["left_x"] | 0;
    m_JoystickData.left_y = doc["left_y"] | 0;
    m_JoystickData.right_x = doc["right_x"] | 0;
    m_JoystickData.right_y = doc["right_y"] | 0;
    m_JoystickData.left_button = doc["left_button"] | false;
    m_JoystickData.right_button = doc["right_button"] | false;
    m_JoystickData.timestamp = millis();
    
    log_d("Received joystick data: L(%d,%d) R(%d,%d) Btns(L:%d,R:%d)", 
          m_JoystickData.left_x, m_JoystickData.left_y,
          m_JoystickData.right_x, m_JoystickData.right_y,
          m_JoystickData.left_button, m_JoystickData.right_button);
    
    // Send success response
    JsonDocument response_doc;
    response_doc["success"] = true;
    response_doc["message"] = "Joystick data received";
    response_doc["timestamp"] = millis();
    
    String response;
    serializeJson(response_doc, response);
    SendResponse(client, response);
}

joystick_data_t CGrpcServer::GetJoystickData()
{
    return m_JoystickData;
}

void CGrpcServer::HandleStreamImuData(WiFiClient& client, String params)
{
    log_i("Starting IMU data streaming for client");
    
    // Parse streaming parameters (rate, duration, etc.)
    JsonDocument paramDoc;
    if (params.length() > 0) {
        DeserializationError error = deserializeJson(paramDoc, params);
        if (!error) {
            m_StreamingRate = paramDoc["rate"] | 10; // Default 10Hz
        }
    }
    
    // Set up streaming
    m_StreamingClient = client;
    m_IsStreaming = true;
    m_LastStreamTime = millis();
    
    // Send initial response
    JsonDocument response_doc;
    response_doc["success"] = true;
    response_doc["message"] = "IMU streaming started";
    response_doc["rate"] = m_StreamingRate;
    response_doc["timestamp"] = millis();
    
    String response;
    serializeJson(response_doc, response);
    SendResponse(client, response);
    
    log_i("IMU streaming started at %d Hz", m_StreamingRate);
}

void CGrpcServer::SendStreamData(WiFiClient& client, String data, bool isLast)
{
    if (!client.connected()) {
        m_IsStreaming = false;
        return;
    }
    
    // Send streaming data with STREAM protocol marker
    String message = "STREAM:" + String(data.length()) + ":" + data;
    if (isLast) {
        message = "STREAM_END:" + String(data.length()) + ":" + data;
        m_IsStreaming = false;
    }
    
    client.println(message);
    client.flush();
    
    log_d("Sent stream data: %d bytes", data.length());
}

void CGrpcServer::SendResponse(WiFiClient& client, String response)
{
    // Send response with simple protocol: LENGTH:DATA
    String message = String(response.length()) + ":" + response;
    client.println(message);
    client.flush();
}