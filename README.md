# ESP32 Rover Control Server

A gRPC-based rover server using Adafruit Feather ESP32-S3 TFT hardware, providing remote control capabilities with dual joystick input processing, real-time IMU data streaming, and LED control.

## 🚀 Features

- **gRPC-like Protocol**: TCP-based server on port 50051 for reliable communication
- **IMU Sensor Integration**: LSM6DSOX 6-axis accelerometer and gyroscope
- **Real-time Data Streaming**: Configurable IMU data streaming (1-50Hz)
- **Joystick Control Processing**: Dual analog joystick input handling
- **LED Control**: NeoPixel LED control with status indication
- **WiFi Access Point**: Creates rover network for client connections
- **Optimized Performance**: Efficient memory usage (17.4% RAM, 54.2% Flash)

## 🛠 Hardware Requirements

### ESP32-S3 Board

- **Adafruit Feather ESP32-S3 TFT** (primary target)
- ESP32-S3 with WiFi capability and TFT display
- Minimum 4MB Flash, 320KB RAM

### Peripheral Components

- **LSM6DSOX IMU**: 6-axis accelerometer and gyroscope sensor
- **NeoPixel LED**: Addressable RGB LED for status indication
- **Power Supply**: USB-C or battery power for mobile operation

## 📡 Communication Protocol

The server provides a gRPC-like TCP service on port **50051** with the following RPC endpoints:

### Supported RPC Services

- `MSG_SET_LED`: Control NeoPixel LED on/off state
- `MSG_GET_IMU`: Return current IMU sensor readings
- `MSG_JOYSTICK_DATA`: Process joystick control commands
- `MSG_STREAM_IMU`: Start/stop continuous IMU data streaming

### Streaming Architecture

- **Protocol**: `STREAM:length:data` format for real-time data
- **Configurable Rate**: 1-50Hz streaming frequency
- **Client Management**: Multiple client support with individual streaming sessions
- **Error Handling**: Automatic cleanup on client disconnect

## 🚀 Getting Started

### Prerequisites

- PlatformIO IDE or CLI
- ESP32 development environment
- LSM6DSOX IMU sensor connected via I2C
- NeoPixel LED connected to GPIO33

### Build and Upload

```bash
# Clone the repository
git clone https://github.com/arunkumar-mourougappane/rover-esp32-control-server.git
cd rover-esp32-control-server

# Build the project
platformio run

# Upload to ESP32-S3
platformio run --target upload

# Monitor serial output
platformio device monitor
```

### Configuration

The server automatically creates a WiFi access point:

- **SSID**: `MOONBASE-II`
- **Password**: `Trypt1c0n$`
- **IP Address**: `192.168.4.1`
- **gRPC Port**: `50051`

## 📋 Pin Configuration

| Component      | Pin  | GPIO   | Description       |
| -------------- | ---- | ------ | ----------------- |
| IMU SDA        | SDA  | GPIO42 | I2C Data Line     |
| IMU SCL        | SCL  | GPIO41 | I2C Clock Line    |
| NeoPixel Data  | DATA | GPIO33 | LED Data Signal   |
| NeoPixel Power | PWR  | GPIO34 | LED Power Control |

## 🎯 Server Capabilities

### IMU Data Processing

The server continuously reads sensor data from the LSM6DSOX:

- **Accelerometer**: ±2g to ±16g range, 3-axis acceleration
- **Gyroscope**: ±125°/s to ±2000°/s range, 3-axis angular velocity
- **Temperature**: Integrated temperature sensor
- **Sampling Rate**: Up to 6.66kHz internal, configurable output rate

### Joystick Command Processing

Processes dual joystick control data:

- **Left Stick**: X/Y axis values (-32768 to 32767)
- **Right Stick**: X/Y axis values (-32768 to 32767)
- **Button States**: Left and right joystick button presses
- **Timestamp**: Command timing for synchronization

### LED Status Indication

NeoPixel LED provides visual feedback:

- **Connection Status**: Color-coded client connection state
- **Command Processing**: Visual confirmation of received commands
- **System Status**: Boot sequence and error indication

## 📚 Library Structure

### Core Libraries

- **GrpcServer**: Main gRPC-like protocol server
- **AccessPointHelper**: WiFi AP management
- **EmbeddedWebServer**: HTTP server (legacy, being phased out)
- **NeoPixel**: LED control library
- **Adafruit LSM6DSOX**: IMU sensor driver

### Key Components

- `GrpcServer.h/cpp`: Protocol server implementation
- `JoystickData.h`: Joystick data structure definitions
- `SensorData.h`: IMU and sensor data structures
- `proto/rover_service.proto`: Protocol service definitions
- `generate_proto.sh`: Protocol documentation generator

## 🔧 Development

### Adding New RPC Services

1. Define new message types in `proto/rover_service.proto`
2. Add handler methods in `GrpcServer` class
3. Update message routing in main server loop
4. Test with client implementation

### IMU Streaming Customization

Configure streaming parameters:

```cpp
// Set streaming rate (Hz)
m_StreamingRate = 20; // 20Hz updates

// Configure IMU sensor range
lsm6ds.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
lsm6ds.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
```

### Debugging

- Serial debug output at 115200 baud
- Comprehensive logging with `log_i()`, `log_e()` macros
- WiFi connection status monitoring
- Client connection tracking

## 📊 Performance Metrics

- **RAM Usage**: 17.4% (57,076 bytes / 327,680 bytes)
- **Flash Usage**: 54.2% (781,553 bytes / 1,441,792 bytes)
- **IMU Sample Rate**: Up to 50Hz streaming
- **Client Connections**: Multiple concurrent clients supported
- **WiFi Range**: Typical ESP32 AP range (~50m)

## 🧪 Testing

### Hardware Testing

1. Connect IMU sensor to I2C pins (GPIO41/42)
2. Connect NeoPixel LED to GPIO33
3. Power up the ESP32-S3
4. Connect client device to `MOONBASE-II` WiFi
5. Test gRPC communication on port 50051

### Protocol Testing

Use the client application or tools like `nc` (netcat) to test:

```bash
# Connect to server
nc 192.168.4.1 50051

# Send LED control command
MSG_SET_LED:{"state": true}

# Request IMU data
MSG_GET_IMU:{}
```

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Implement and test changes
4. Update documentation
5. Submit a pull request

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.
